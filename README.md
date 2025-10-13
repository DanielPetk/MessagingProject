# Terminal Chat

Server and client of a cross-platform terminal-based group chat application.

## Features

- Chat server that allows clients to connect and transfers messages between them, with logging and configuration options

- Chat client that connects to the chat server to send messages with support for custom usernames

- Support for any number of users in a group chat

- Cross-platform* (read **Disclaimers** section)

- One dependancy

## About

This is a personal project I wrote to experiment with multithreading and socket programming. I thought of the idea after I learned about these concepts in class and decided to give it a try. I wrote a rough version of this project in a few days (`legacy` branch) and it interested me enough to push me to write something slightly bigger.

I chose C++ for this project since it is the language I am by far the most familiar with and I wanted to practice managing memory and resources on my own (insead of using something like Python, which would have been much easier :) ). For the user interface, I used [FTXUI](https://github.com/ArthurSonzogni/FTXUI), since I did not want the project to focus on writing terminal UI code from scratch. I used Winsock and Berkely Sockets for Windows and Linux respectively to do the socket programming, with TCP as the protocol. I just used C++'s native threading to do multithreading.


## Disclaimers

Since this is a personal project created mainly for learning purposes, it is rough around the edges. Here are some things to look out for:

- TCP is a stream protocol, however I treat every call of `recv` as an individual message for simplicity. This works well enough since a message is normally handled before the next message comes in, but this could techincally cause problems.

# WIP ......