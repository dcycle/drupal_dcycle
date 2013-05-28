***** HOW TO USE DCYCLE *****

**** INITIAL SETUP ****

- Make sure you have access to the command line; this works on Mac or Linux. Windows is not supported.
- You should have some knowledge of how to use drush and Drupal locally.
- You should have some programming knowledge.
- Make sure drush is installed and in your PATH, see http://drupal.org/project/drush for details. Typing "which drush" should give you a path.
- Make sure git is intalled and in your PATH. Typing "which git" should give you a path.

**** QUICK START ****

(Takes about an hour)

- create a new Drupal 7 installation (dcycle1313) on your computer.

- install and enable dcycle

- visit admin/config/development/dcycle

- select dCycle Quickstart as your policy

- the environment type will be set to Development by default, which is what you need when starting a project. 

- the project type will be set to website by default; you can also set the project type to module or theme.

- If you are developing a website, create a deployment module at sites/default/modules/custom/[namespace]_deploy. [namespace] should contain only lowercase letters.

- If you are developing a module or theme, create it and let dCycle know what it is.

- Function and unit testing of javascript fallback: your deployment module will need a simpletest to go with it. Follow the instructions on-screen.

- Now go to the command line and type:

drush dcycle-test

- Each time, dcycle will go through a preflight and tell you what remains to do. Once ready, all your tests should pass.

- Get the full path to your drush installation. Type:

which drush

and take note of it (for example /full/path/to/drush/drush)

- Now install jenkins on your computer

- Switch to your jenkins user. Go to the command line and type:

sudo su jenkins

- As jenkins, type

/full/path/to/drush/drush dcycle-test

- Follow instructions to also make it run.

- Go to admin/config/development/dcycle/jenkins, and follow the instructions therein to create a new Jenkins project which tracks dCycle.

- Jenkins should automatically run, or you can click "Build now".

- Troubleshoot until the build passes [link to dcycle issue queue]

- When the build passes, jenkins will update the master branch of your project.

- Make sure to insert a bug in your project somewhere, push to master (it is rejected), then go to jenkins and click build now in your project.

- Notice that master is not updated, you can look at the failed build info figure out why.

- Remove the bug and insert some new feature in your project, then push to master, then go to jenkins and build now.

- Jenkins will automatically update master when all the tests pass.

** MORE DCYCLE: MOCK OBJECTS, PEER REVIEW, BEHAVIOUR TESTS, DEVOPS, & IE **

(takes about a day)

- Go to admin/config/development/dcycle, and change your policy to dCycle full

- In the mock objects section, click New mock object... and follow the instructions on-screen. Go through the examples once until you are comfortable with the concepts.

- Enforcing peer reviews: Jenkins will enforce Peer reviews and contact by IRC.

- Jenkins full requires at least one Behat feature file, install it by following on-screen instructions.

** CREATING NEW POLICIES **

- Go to admin/config/development/dcycle, and, in the policy menu, select Create new policy...

- Follow instructions
