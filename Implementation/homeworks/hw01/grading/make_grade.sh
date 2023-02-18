#!/usr/bin/env bash
# File       : make_grade.sh
# Description: Generate your exercise grade
# Copyright 2021 ETH Zurich. All Rights Reserved.
#
# EXAMPLE:
# python grade.py \
#     --question1 0 \
#     --comment1 'Add a comment to justify your score' \
#     --question2 0 \
#     --question2 0 \
#
# FOR HELP:
# python grade.py --help
#
# The script generates a grade.txt file. Submit your grade on Moodle:
# https://moodle-app2.let.ethz.ch/course/view.php?id=13666

# Note: --question2 and --question5 are not graded
python3 grade.py \
    --question1 20 \
    --comment1 'Add a comment to justify your score 1a)' \
    --comment1 'Add a comment to justify your score 1-2' \
    --question2 20 \
    --comment2 'Add a comment to justify your score 3' \
    --question3 12 \
    --comment3 'Add a comment to justify your score 4' \
