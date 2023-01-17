# Szkolenie - Zaawansowane programowanie w C++ #

## Dokumentacja + slajdy

* https://infotraining.bitbucket.io/cpp-adv
* https://infotraining.bitbucket.io/cpp-adv/slides/

## Konfiguracja środowiska

### Lokalna

Przed szkoleniem należy zainstalować na swoim:

#### Kompilator + CMake

* Dowolny kompilator C++ (gcc, clang, Visual C++) wspierający C++17
  * polecany kompilator pod Windows (https://nuwen.net/mingw.html) - instalacja polega na rozpakowaniu archiwum do katalogu C:\MinGW. Szczegółowy opis instalacji znajduje się na stronie.

* [CMake > 3.15](https://cmake.org/)
  * proszę sprawdzić wersję w lini poleceń

  ```
  cmake --version
  ```

#### Visual Studio Code

* [Visual Studio Code](https://code.visualstudio.com/)
* Zainstalować wtyczki
  * C/C++ Extension Pack
  * Live Share

### Docker + Visual Studio Code

Jeśli uczestnicy szkolenia korzystają w pracy z Docker'a, to należy zainstalować:

#### Visual Studio Code

* [Visual Studio Code](https://code.visualstudio.com/)
* Zainstalować wtyczki
  * Live Share
  * Dev Containers ([wymagania](https://code.visualstudio.com/docs/devcontainers/containers#_system-requirements))
    * po instalacji wtyczki - należy otworzyć w VS Code folder zawierający sklonowane repozytorium i
      z palety poleceń (Ctrl+Shift+P) wybrać opcję **Dev Containers: Rebuild and Reopen in Container**

