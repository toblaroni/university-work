from django.contrib import admin
from .models import Professor, Module, ModuleInstance, Rating

@admin.register(Professor)
class ProfessorAdmin(admin.ModelAdmin):
    list_display = ('id', 'name')

@admin.register(Module)
class ModuleAdmin(admin.ModelAdmin):
    list_display = ('code', 'name')

@admin.register(ModuleInstance)
class ModuleInstanceAdmin(admin.ModelAdmin):
    list_display = ('module', 'year', 'semester')

@admin.register(Rating)
class RatingAdmin(admin.ModelAdmin):
    list_display = ('user', 'professor', 'module_instance', 'rating')

