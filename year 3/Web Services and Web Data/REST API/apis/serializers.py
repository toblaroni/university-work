from rest_framework import serializers
from django.contrib.auth.models import User
from .models import Professor, Module, ModuleInstance, Rating
from rest_framework.validators import UniqueValidator

# https://medium.com/@dhanushpathiprakash/building-a-user-authentication-in-django-rest-framework-a-step-by-step-guide-53729a91cced
class UserSerializer(serializers.ModelSerializer):
    email = serializers.EmailField(required=True, validators=[
        UniqueValidator(queryset=User.objects.all())
    ])

    class Meta:
        model = User
        fields = ('id', 'username', 'email', 'password')

    def create(self, validated_data):
        user = User.objects.create_user(**validated_data)
        return user


class ProfessorSerializer(serializers.ModelSerializer):
    class Meta:
        model = Professor
        fields = '__all__'


class ModuleSerializer(serializers.ModelSerializer):
    class Meta:
        model = Module
        fields = '__all__'


class ModuleInstanceSerializer(serializers.ModelSerializer):
    module = ModuleSerializer()
    professors = ProfessorSerializer(many=True)

    class Meta:
        model = ModuleInstance
        fields = '__all__'

class RatingSerializer(serializers.ModelSerializer):
    # Since serializers are used for JSON to objects, we can validates incoming requests here
    # https://www.django-rest-framework.org/api-guide/serializers/#object-level-validation
    def validate(self, data):
        if data['rating'] < 1 or data['rating'] > 5:
            raise serializers.ValidationError("Rating must be between 1 and 5.")

        # We also want to make sure the professor is actually in the moduleInstance's professors
        module_instance = data['module_instance']
        professor = data['professor']

        # Search for the professor in the instance's professors
        if not module_instance.professors.filter(id=professor.id).exists():
            raise serializers.ValidationError("This professor did not teach this module instance.")

        return data


    class Meta:
        model = Rating
        fields = '__all__'
        unique_together = ('user', 'professor', 'module_instance')
