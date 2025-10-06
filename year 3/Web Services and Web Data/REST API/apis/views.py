from django.contrib.auth import authenticate
from django.middleware.csrf import get_token
from django.db.models import Avg

from rest_framework.parsers import JSONParser
from rest_framework.authtoken.models import Token
from rest_framework.decorators import permission_classes, authentication_classes, api_view
from rest_framework.permissions import IsAuthenticated
from rest_framework.authentication import TokenAuthentication
from rest_framework.response import Response
from rest_framework import status

from .serializers import (
    UserSerializer,
    ModuleInstanceSerializer,
    RatingSerializer
)

from .models import (
    Professor,
    Module,
    ModuleInstance,
    Rating
)


# This is where the logic for handling requests is implemented
# https://www.django-rest-framework.org/tutorial/1-serialization/
# https://blog.webdevsimplified.com/2022-12/http-status-codes/
# This is needed for grabbing the csrf token
@api_view(['GET'])
def handleGetTokenRequest(request):
    return Response({ "csrf_token": get_token(request) })


@api_view(['POST'])
def handleRegisterRequest(request):
    try:
        data = JSONParser().parse(request)
    except ValueError:
        return Response({ "detail": "Invalid JSON" }, status=status.HTTP_400_BAD_REQUEST)

    serializer = UserSerializer(data=data)

    # Check request using User serializer
    if serializer.is_valid():
        serializer.save()
        return Response({ "detail": "Registration Successful."}, status=status.HTTP_201_CREATED)

    fmtErrors = format_serializer_errors(serializer.errors)
    return Response({ "detail": fmtErrors }, status=status.HTTP_400_BAD_REQUEST) 


# https://docs.djangoproject.com/en/5.1/topics/auth/default/#django.contrib.auth.login
# https://www.django-rest-framework.org/api-guide/authentication/
@api_view(['POST'])
def handleLoginRequest( request ):
    try:
        data = JSONParser().parse(request)
    except ValueError:
        return Response({ "detail": "Invalid JSON" }, status=status.HTTP_400_BAD_REQUEST)

    username = data.get("username")
    password = data.get("password")

    if not username or not password:
        return Response(
            { "detail": "Please provide a username and a password" }, 
            status=status.HTTP_400_BAD_REQUEST
        )
    
    user = authenticate(request=request, username=username, password=password)
    if user is not None:
        # This token allows 
        token, _ = Token.objects.get_or_create(user=user)
        return Response({ 
            "detail": "Login Successful", 
            "token": token.key 
        })
    
    return Response({ "detail": "Invalid credentials" }, status=status.HTTP_401_UNAUTHORIZED) # Unauthorized


@api_view(['POST'])
@authentication_classes([TokenAuthentication])
@permission_classes([IsAuthenticated])
def handleLogoutRequest( request ):
    if not request.user.is_authenticated:
        return Response(
            { "detail": "User is not authenticated." }, 
            status=status.HTTP_401_UNAUTHORIZED
        )

    request.user.auth_token.delete()
    return Response({"detail": "Logout successful"})


@api_view(['GET'])
def handleListRequest( request ):
    # Get all module instances from the database
    moduleInstances = ModuleInstance.objects.all()
    serializer = ModuleInstanceSerializer(moduleInstances, many=True)

    return Response({ 
        "detail": "Successfully fetched module instances", 
        "module_instances": serializer.data 
    })


# Returns average rating of professors across all modules
@api_view(['GET'])
def handleViewRequest( request ):
    results = []
    for professor in Professor.objects.all():   # Loop through all professors
        # Using aggregate we can get the average per professer
        avgRating = Rating.objects.filter(
            professor__id=professor.id
        ).aggregate(average_rating=Avg('rating'))

        avgRating = avgRating["average_rating"]

        rating = int(round(avgRating)) if avgRating else None

        results.append({
            "professor_name": professor.name,
            "professor_id": professor.professor_id,
            "rating": rating
        })

    return Response({ 
        "detail": "Successfully fetched module instances", 
        "professor_ratings": results
    })


@api_view(['GET'])
def handleAverageRequest( request ):
    try:
        data = JSONParser().parse(request)
        professor_id = data['professor_id']
        module_code = data['module_code']
    except (ValueError, KeyError):
        return Response(
            { "detail": "Expected 'professor_id' and 'module_code' in JSON format." }, 
            status=status.HTTP_400_BAD_REQUEST
        )

    try:
        # https://docs.djangoproject.com/en/5.1/ref/models/querysets/#get
        professor = Professor.objects.get(professor_id=professor_id)
        module = Module.objects.get(code=module_code)
    except (Professor.DoesNotExist, Module.DoesNotExist):
        return Response(
            { "detail": "Error: 'module_code' or 'professor_id' is incorrect. Check values and try again."},
            status=status.HTTP_400_BAD_REQUEST
        )

    # Test if there is an instance where that professor teaches that module
    if not ModuleInstance.objects.filter(module=module.id, professors=professor.id).exists():
        return Response(
            { "detail": "Chosen professor has never taught that module." },
            status=status.HTTP_400_BAD_REQUEST
        )

    # https://docs.djangoproject.com/en/5.1/ref/models/querysets/#id6
    avgRating = Rating.objects.filter(
        professor=professor, 
        module_instance__module=module
    ).aggregate(average_rating=Avg('rating'))['average_rating']

    data = {
        "professor_name": professor.name,
        "professor_id": professor.professor_id,
        "module_name": module.name,
        "module_code": module.code,
        "average_rating": int(round(avgRating)) if avgRating else None
    }

    return Response({ 
        "detail": f"Successfully fetched average rating for {professor_id}, {module_code}",
        "average_rating": data
    })


@api_view(['POST'])
@authentication_classes([TokenAuthentication])
@permission_classes([IsAuthenticated])
def handleRateRequest( request ):
    try:
        data = JSONParser().parse(request)
        
        # Extract data
        professor_id = data["professor_id"]
        module_code = data["module_code"]
        year = data["year"]
        semester = data["semester"]
        rating = data["rating"]
        
        professor = Professor.objects.get(professor_id=professor_id)
        
        moduleInstance = ModuleInstance.objects.get(    # There should only be one
            module__code=module_code,
            year=year,
            semester=semester,
            professors__professor_id=professor_id    # This matches many-to-many 
        )

        # Now we need to create a rating. We can use our serializer to validate and create
        try:
            rating = int(round(float(rating)))  # Do this to round properly
        except (ValueError, TypeError):
            return Response({ "detail": "Rating must be integer value" }, status=status.HTTP_400_BAD_REQUEST)

        serializer = RatingSerializer(data={
            "user": request.user.id,
            "professor": professor.id,
            "module_instance": moduleInstance.id,
            "rating": rating
        })

        if serializer.is_valid():
            serializer.save()
            return Response({"detail": "Rating created successfully."}, status=status.HTTP_201_CREATED)
        else:
            fmtError = format_serializer_errors(serializer.errors)
            return Response( { "detail": fmtError }, status=status.HTTP_400_BAD_REQUEST)
        
    except ValueError:
        return Response({ "detail": "Invalid JSON." }, status=status.HTTP_400_BAD_REQUEST)
    except KeyError:
        return Response({ "detail": "Request is missing required keys." }, status=status.HTTP_400_BAD_REQUEST)
    except ModuleInstance.DoesNotExist:
        return Response({ "detail": "Module instance could not be retrieved." }, status=status.HTTP_400_BAD_REQUEST)
    except Professor.DoesNotExist:
        return Response({ "detail": "Professor could not be retrieved." }, status=status.HTTP_400_BAD_REQUEST)
    except ModuleInstance.MultipleObjectsReturned:
        # This shouldn't happen 
        return Response(
            { "detail": "Internal Error: Multiple instances were matched."},
            status=status.HTTP_500_INTERNAL_SERVER_ERROR    # This is our fault, not the client
        )

def format_serializer_errors(errors):
    error_messages = []
    for field, field_errors in errors.items():
        for error in field_errors:
            print(error, field_errors)
            error_messages.append(f"\t{field}: {error}\n")
    return "".join(error_messages)