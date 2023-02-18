# Exercise 1

Moodle:
[EXERCISE/GRADE SUBMISSION](https://moodle-app2.let.ethz.ch/course/view.php?id=15876)

## Goals

* Get familiar with Amdahl's Law
* Get familiat with Roofline model
* Learn about cache and cache optimizations

## Grading

You can grade the exercise by using the `grade.py` grading script (the script
will be added to the repository together with the solution).  You determine
your point score by grading the exercise yourself, detailed point distributions
are provided with the exercise solutions.  You then create your grade file by
executing the `grade.py` script.

The following is an example with 2 questions to be graded:

```bash
python grade.py \
    --question1 12 \
    --comment1 'I justify my point score because of this and that' \
    -q2 20 \
    -c2 'I made an assumption in part b' \
    -c2 'Part c did not converge'
```

Explanation:

* Pass your point score for the question `X` using the `--questionX` argument.
  Alternatively, you can use the short form `-qX`.  The arguments accepts an
  _integer_ value.  The default value if omitted is `0`.
* **Optional**: You can decorate question `X` with comments by using the
  `--commentX` or `-cX` argument.  You may use this argument multiple times to
  pass more than one comment if you like.  The argument accepts a _string_, be
  sure to quote them correctly.
* Use `python grade.py --help` for help.

For your convenience, we provide a `make_grade.sh` BASH script where you can
fill in your point scores and save the file for your later reference.  The
exercise grades are determined with by a linear scale.  Zero points is a
grade 3.0, maximum points is a 6.0.

### Hand-in your grade

The script generates a `grade.txt` file for the exercise.  You hand-in your
grade by **submitting** this file on Moodle under the 'Grading' section.
