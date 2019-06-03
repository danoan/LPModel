#List of directories in which cmake will look up
#for packages by means of find_package utility.
#find_package is called for DGtal and openCV
#libraries
FIND_PACKAGE_EXTRA_LOOK_UP_DIRS=$1
BOOST_LIBS_DIR=$2

sh build-scripts/clone-repos.sh
sh build-scripts/make-ext-project.sh $FIND_PACKAGE_EXTRA_LOOK_UP_DIRS $BOOST_LIBS_DIR release