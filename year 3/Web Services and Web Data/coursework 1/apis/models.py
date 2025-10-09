from django.db import models
from django.contrib.auth.models import User

class Professor(models.Model):
    professor_id = models.CharField(max_length=15, unique=True)  # Example: "JE1"
    name = models.CharField(max_length=255)

    def __str__(self):
        return f"{self.name}"

class Module(models.Model):
    code = models.CharField(max_length=15, unique=True)
    name = models.CharField(max_length=255)

    def __str__(self):
        return f"{self.code} - {self.name}"

class ModuleInstance(models.Model):
    module = models.ForeignKey(Module, on_delete=models.CASCADE)
    year = models.IntegerField()
    semester = models.IntegerField()
    professors = models.ManyToManyField(Professor)

    class Meta:
        unique_together = ('module', 'year', 'semester')

    def __str__(self):
        return f"{self.module.code} ({self.year}, Semester {self.semester})"

class Rating(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    professor = models.ForeignKey(Professor, on_delete=models.CASCADE)
    module_instance = models.ForeignKey(ModuleInstance, on_delete=models.CASCADE)
    rating = models.IntegerField()

    class Meta:
        # A user can only rate a module instance once
        unique_together = ( 'user', 'professor', 'module_instance' )

    def __str__(self):
        return f"{self.user.username} rated {self.professor.name} {self.rating}/5"

