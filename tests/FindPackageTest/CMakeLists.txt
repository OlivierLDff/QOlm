# How can I use this ?
#
# Well, license here is pretty "super-open", not even copy-left or copy-right,
# basically its close to the WTFPL, just use it as you want, as it's the most practical to you :
#
# * If you want to use it as GIT submodule and compile it in your app, do it.
# * If you prefer separate project as a shared library, do it.
# * If you need to modify the project to be able to integrate in you app (opensource or not), do it.
# * If you want to share you work on the library, thanks a lot, but if you don't, no problem.
# * If you think about some weird case I didn't talk about, well, do whatever you want, I don't need to know about it.
# * If you like it so much you want to spread the word, thank you, you're welcome.
#
# Enjoy !

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(QOlmFindPackageTest)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Qml REQUIRED)
find_package(QOlm REQUIRED)

add_executable(QOlmFindPackageTest main.cpp)
target_link_libraries(QOlmFindPackageTest QOlm::QOlm)
