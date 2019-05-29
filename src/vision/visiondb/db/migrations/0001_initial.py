# Generated by Django 2.1.7 on 2019-04-17 01:24

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Character',
            fields=[
                ('id', models.IntegerField(primary_key=True, serialize=False, unique=True)),
                ('char', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Color',
            fields=[
                ('id', models.IntegerField(primary_key=True, serialize=False, unique=True)),
                ('color', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Data',
            fields=[
                ('id', models.IntegerField(primary_key=True, serialize=False, unique=True)),
                ('Lat', models.FloatField()),
                ('Lng', models.FloatField()),
                ('Alt', models.FloatField()),
                ('Heading', models.IntegerField()),
                ('Modified', models.BooleanField()),
                ('PipeStage', models.TextField()),
                ('img', models.BinaryField()),
            ],
        ),
        migrations.CreateModel(
            name='Orientation',
            fields=[
                ('id', models.IntegerField(primary_key=True, serialize=False, unique=True)),
                ('orientation', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Shape',
            fields=[
                ('id', models.IntegerField(primary_key=True, serialize=False, unique=True)),
                ('shape', models.IntegerField()),
            ],
        ),
    ]
