#!/bin/bash
if [ ! -f plantuml.jar ];
then
  wget http://sourceforge.net/projects/plantuml/files/plantuml.jar
fi
java -jar plantuml.jar *.uml