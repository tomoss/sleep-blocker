# Sleep Blocker

![App Preview](docs/app-preview.png)

[![C++20](https://img.shields.io/badge/dialect-C%2B%2B20-blue)](https://en.cppreference.com/w/cpp/20)
[![Build](https://github.com/tomoss/sleep-blocker/actions/workflows/build.yml/badge.svg)](https://github.com/tomoss/sleep-blocker/actions/workflows/build.yml)
[![Windows](https://img.shields.io/badge/platform-Windows-0078D4?logo=windows)]()
[![Linux](https://img.shields.io/badge/platform-Linux-FCC624?logo=linux&logoColor=black)]()


**Sleep Blocker** is a simple cross-platform desktop app for Windows and Linux, written in C++, that keeps your computer awake by temporarily preventing system sleep using native OS APIs.

## Goals

This mini project started as a way to:

* Build a single sleep-blocking app for personal use that works on both Windows and Linux.
* Experiment with the [Dear ImGui](https://github.com/ocornut/imgui) library for building lightweight desktop UIs.
* Explore platform-specific APIs for temporarily preventing system sleep.