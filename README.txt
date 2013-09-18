How to use Dcycle, a building code for Drupal
=============================================

Intro
-----

Dcycle is meant to help you enforce best practices and get team members up and running fast(er) with automated testing and continuous integration.

The idea of Dcycle is that:
* you develop on an unstable branch
* a Jenkins server polls git for changes to your unstable branch
* if drush dcycle-test yields no errors, your change is pushed to a (more) stable branch

Initial setup
-------------

* Make sure you have access to the command line; this works on Mac or Linux. Windows is not supported.
* You should have some knowledge of how to use drush and Drupal locally.
* You should have some programming knowledge.
* Make sure drush is installed and in your PATH, see http://drupal.org/project/drush for details. Typing "which drush" should give you a path.
* Make sure git is intalled and in your PATH. Typing "which git" should give you a path.

Example usecase
---------------

* You are developing a module xyz on a branch 7.x-1.x-unstable
* A jenkins job runs some tests and moves the code to 7.x-1.x if tests pass

Your jenkins job commands might look like:

    # make sure we have access to crush
    export PATH=/path/to/drush:$PATH
    
    # run our tests, using cc all to add new ones if necessary
    drush cc all
    drush test-run MyModule
    
    # make sure we have a nice coding style
    drush coder-review --minor --comment mymodule mymodule_ui

The above poses two problems:

* First, Drush will not trigger an error if test-run or coder-review fail.
* Second, the above code is not in version control.

Enter Dcycle
------------

Dcycle allows the following advantages:

* put the above code in your version control repo where it belongs, and not in your Jenkins project.
* You can also put fail conditions directly in your code: for example, should warnings in tests trigger a fail of the build?

Example
-------

In the above example, you would add a file called mymodule.dcycle.inc to your project, and in it put your ci code:

    function mymodule_dcycle_test() {
      return array(
        'commands' => array(
          'Run Simpletests' => array(
            'commands' => array(
              // cc all because otherwise the environment might not
              // pick up new tests in the MyModule group
              'drush cc all',
              'drush test-run MyModule',
            ),
            'fail' => array(
              'grep' => array(
                '[1-9] fails',
                '[1-9][0-9]* fails',
                '[1-9] exceptions',
                '[1-9][0-9]* exceptions',
              ),
            ),
          ),
          'Run Code Review' => array(
            'commands' => array(
              'drush coder-review --minor --comment mymodule mymodule_ui',
            ),
            'fail' => array(
              'grep' => array(
                '[1-9] normal warnings',
                '[1-9][0-9]* normal warnings',
                '[1-9] minor warnings',
                '[1-9][0-9]* minor warnings',
              ),
            ),
          ),
        ),
      )
    }

You don't need to make dcycle a dependency of your module. But, in your CI server, run:

    # make sure we have access to crush
    export PATH=/path/to/drush:$PATH
    
    # run our tests, running dcycle-test runs tests defined for our module
    drush en dcycle
    drush dcycle-test mymodule
