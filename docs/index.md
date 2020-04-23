---
layout: default
title: Index
nav_exclude: true
---

# QOlm Documentation

Bring `QOlm::QOlm` data type based on `QAbstractListModel` that provide a list of `QObject` based class to **js** and **c++**. The goal of this list is to dynamically update views by reacting to **insert**, **remove**, **move** operations.
QOlm stands for QObjectListModel

## Overview


<p align="center">
  <img src="./ClassDiagram.svg"/>
</p>


`QOlm` is based on `QAbstractListModel`, and behave as a list of custom `QObject`. `QOlmBase` is detail implementation to provide `signals` and `slots` that `moc` can handle. Since `moc` isn't working with template class.

- Checkout [Quick Start Guide](https://olivierldff.github.io/QOlm/quickstartcpp.html) to start with cpp.
- Then read how to use QOml [from Qml](https://olivierldff.github.io/QOlm/quickstartqml.html)
- And check how to (build the library)[https://olivierldff.github.io/QOlm/build.html].