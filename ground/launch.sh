#!/bin/sh

# Run server and React
# todo: React should be syncing automatically during front-end development - needs to be resolved - must fix webpack

python "client/build.py" &&
	python "run_ground.py"