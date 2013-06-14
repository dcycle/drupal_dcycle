How to use dcycle, a building code for Drupal
=============================================

Intro
-----

Dcycle is meant to help you enforce best practices and get team members up and running fast(er) with automated testing and continuous integration.

The idea of dcycle is that:
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

Quick start (takes about an hour)
---------------------------------

Dcycle demo -- developing a website with dcycle

Step 1: install Drupal locally

* Make sure you have a webserver on your computer (this example uses MAMP)
* Download Drupal so it is available locally
    cd /Applications/MAMP/htdocs
    drush dl
    drush dl; mv drupal-7* dcycle
* Create a database for your local site
    echo 'create database dcycle' | mysql -uroot -proot
* Install Drupal
    cd /Applications/MAMP/htdocs/dcycle
    drush si --db-url=mysql://root:root@localhost/dcycle --account-name=root --account-pass=root -y
* Make sure you can access your site locally, for example at http://localhost:8888/dcycle

Step 2: get dcycle (note: it is not yet available with drush because we're in sandbox mode)

* Download dcycle
    cd sites/all/modules
    git clone --branch 7.x-1.x alberto56@git.drupal.org:sandbox/alberto56/1974172.git dcycle
* Enable dcycle
    drush en dcycle -y

Step 3: Setup dcycle in the UI

* Log into your local site: get a onetime login link and paste it into your browser
    drush uli
* Visit the dcycle admin page, for example at http://localhost:8888/dcycle/admin/config/development/dcycle
* select "website" for development type

Step 4: Download required modules, and make dcycle aware of your deployment module. To use dcycle, all websites need to have a deployment module located at sites/*/modules/custom/mysite_deploy, where mysite is a namespace unique to your project.

* enable the required Coder module:
    drush dl coder;drush en coder_review -y;
* create your deployment module
    sudo chmod 755 sites/default
    cd sites/default
    mkdir -p modules/custom/mysite_deploy
* create some files required by your module
    touch modules/custom/mysite_deploy/mysite_deploy.info
    touch modules/custom/mysite_deploy/mysite_deploy.install
    touch modules/custom/mysite_deploy/mysite_deploy.test
    touch modules/custom/mysite_deploy/mysite_deploy.module
* in mysite_deploy.info put:
    name = My Site Deployment
    core = 7.x
    files[] = mysite_deploy.test
* in mysite_deploy.module put:
    <?php
* in mysite_deploy.test put:
    /**
     * @file
     * Test what this site should do.
     */
     
    /**
     * Test case for the site one-step deployment process
     */
    class MySiteDeployTestCase extends DrupalWebTestCase {
      /**
       * Info for the test case
       */
      public static function getInfo() {
        return array(
          'name' => 'My Site Deploy Test Case',
          'description' => 'My Site Deploy Test Case.',
          'group' => 'mysite deploy',
        );
      }
    
      /**
       * Setup to be run before each test
       */
      public function setUp() {
        parent::setUp('mysite_deploy');
      }
    
      /**
       * Main test for this test case.
       */
      public function test() {
        $this->drupalGet('/');
        $this->assertText('Welcome', 'Welcome text present');
        $this->assertTrue(1+1 == 2, '1+1 is 2');
      }
    }
* enable your module
    drush en mysite_deploy -y
* tell dcycle what you are testing by adding this to sites/default/settings.php
    $conf['dcycle'] = array(
      'modules' => array(
        'mysite_deploy' => array(),
      ),
    );

Step 5: launch dcycle-test on the command line

* enable simpletest and launch your first test on the command line
    drush en simpletest -y
    drush dcycle-test
* drush dcycle-test will let you know of certain problems to fix. Most common are:
** you forgot to enable simpletest
** you need to set the base_url in sites/default/settings.php: open that file, find $base_url, uncomment it and put your actual url there, for example:
    $base_url = 'http://localhost:8888/dcycle';
* Run drush dcycle-test repeatedly and all errors you find, including coder review missing docblock (for example, certain @file blocks will be missing if you have been following these steps) until drush dcycle-test gives you no errors.

Step 6: put your project in git

* Make sure none of the folders in your project are git folders. For example, if you have downloaded dcycle via git, you might want to remove the .git folder, or else git will get confused and think you are using submodules. If you really want to use git submodules, omit this step:
    cd /Applications/MAMP/htdocs/dcycle
    mv sites/all/modules/dcycle/.git sites/all/modules/dcycle/_git
* Make your entire website a git repo (dcycle only supports this method of version control for now):
    cd /Applications/MAMP/htdocs/dcycle
    git init
    git add .
    git commit -am 'initial commit'

Step 7: link your project to a remote repo

* Create a remote git repo for your site (with github for example), take note of the git address, for example git@github.com:mygithubhandle/mygitproject.git
* Make sure the remote git repo is aware of your public ssh key
* Link your local git repo to the remote and push your master branch there
    git remote add origin git@github.com:mygithubhandle/mygitproject.git
    git push origin master

Step 8: link your project to a continuous integration server. For this example we'll put Jenkins directly on our local computer

* Download and install Jenkins, and make sure you have access to it, for example on http://localhost:8080
* Install the git extension to Jenkins
* Make sure your jenkins user's ssh key is recognized by your git repo
* Now under your jenkins user, go to a path which you can access via the web and git clone your git repo to that path.
    sudo su jenkins
    cd /Applications/MAMP/htdocs
    git clone git@github.com:alberto56/dcycle.git dcycle_ci
* Create a database to use for jenkins
    echo 'create database dcycle_ci' | mysql -uroot -proot
* Setup your jenkins site
    drush si --db-url="mysql://root:root@localhost/dcycle_ci" -y
    chmod u+w sites/default/settings.php
    echo '$conf[\'dcycle\'] = array(\'modules\' => array(\'mysite_deploy\' => array()));' >> sites/default/settings.php
    echo '$base_url = \'http://localhost:8888/dcycle_ci\';' >> sites/default/settings.php
* In Jenkins create a new project (i.e. dcycle) of type freestyle
* In source code management put your git address, and, under "advanced", name your repo "main" (you will use this in a later step).
* The build trigger should be poll every minute (* * * * *)
* Advanced project options: add a custom workspace which you can access via the web (e.g. on a mac with MAMP, you might use something like /Applications/MAMP/htdocs/dcycle_ci
* Add a build step Execute shell:
    export PATH=$PATH:/Users/tp1/scripts/drush
    drush si --db-url="mysql://root:root@localhost/dcycle_ci" -y
    drush en coder -y
    drush en simpletest -y
    drush en mysite_deploy -y
    drush en dcycle -y
    drush dcycle-test
* Add a post-build action of type Post-build task, and make sure Jenkins adds some data to your deployment module's .info file if all goes well, and check "Run script only if previous steps were successful":
    cat sites/default/modules/custom/mysite_deploy/mysite_deploy.info | grep -v 'version' | grep -v ';following information inserted by the dcycle jenkins CI server' > sites/default/modules/custom/mysite_deploy/mysite_deploy.info.swp
    echo ';following information inserted by the dcycle jenkins CI server' >> sites/default/modules/custom/mysite_deploy/mysite_deploy.info.swp 
    echo 'version =' $(date +%Y-%m-%d-%H:%M:%S) >> sites/default/modules/custom/mysite_deploy/mysite_deploy.info.swp 
    rm sites/default/modules/custom/mysite_deploy/mysite_deploy.info
    mv sites/default/modules/custom/mysite_deploy/mysite_deploy.info.swp sites/default/modules/custom/mysite_deploy/mysite_deploy.info
    git commit -am $(date +%Y-%m-%d-%H:%M:%S)
* Finally add a post-build step to move the contents of master to the branche stage if all went well. For this you will use the git publisher post-build step, and you can push to branches master and stage, and enter "main" in Target remote name (if you haven't yet saved, you will get a warning message which you can ignore).
