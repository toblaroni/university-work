"""
URL configuration for Professorater project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/5.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path, include
from apis.views import (
    handleGetTokenRequest,
    handleRegisterRequest,
    handleLoginRequest,
    handleLogoutRequest,
    handleListRequest,
    handleViewRequest,
    handleAverageRequest,
    handleRateRequest
)

urlpatterns = [
    path('admin/', admin.site.urls),
    path('api/csrf-token/', handleGetTokenRequest),
    path("api/register/", handleRegisterRequest),
    path("api/login/", handleLoginRequest),
    path("api/logout/", handleLogoutRequest),
    path("api/list/", handleListRequest),
    path("api/view/", handleViewRequest),
    path("api/average/", handleAverageRequest),
    path("api/rate/", handleRateRequest),
]
