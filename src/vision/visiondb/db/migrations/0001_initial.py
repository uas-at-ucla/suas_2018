# Generated by Django 2.1.7 on 2019-02-14 01:36

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Character',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('char', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Color',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('color', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Data',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
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
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('orientation', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Shape',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('shape', models.IntegerField()),
            ],
        ),
    ]
