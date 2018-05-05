falsh is a unix shell interpreter. It has built in commands and can
call other commands. Use simply by running falsh or ./falsh

Built-in Commands:
 - exit
      Exits the program, no parameters
 - pwd
      Prints current working directory, no paramters
 - cd [dir]
      Change directory
      No parameters changes to home directory
      Single optional paramters changes to specified path
 - setpath <dir> [dir] ... [dir]
      Overwrites the path to one or more specified paths
      DOES NOT check to make sure paths are valid
 - help
      Displays this screen
