[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![LGPL License][license-shield]][license-url]


<br />
<p align="center">
  <a href="https://github.com/formicidae-tracker/tag-scanner">
    <img src="src/resources/icons/flaticon.com/freepik/icon.svg" alt="Logo" width="180" height="180">
  </a>

  <h1 align="center">FORT Tag Scanner</h1>

  <p align="center">
    Manual Tag Scanner for the FORmicidae Tracker
    <br />
    <a href="https://github.com/formicidae-tracker/documentation/wiki">FORT Project Wiki</a>
    ·
    <a href="https://github.com/formicidae-tracker/tag-scanner/issues">Report Bug</a>
    ·
    <a href="https://github.com/formicidae-tracker/tag-scanner/issues">Request Feature</a>
  </p>
</p>

## About the project
[![Product Name Screen Shot][product-screenshot]](https://github.com/formicidae-tracker/tag-scanner)

FORT Tag Scanner is a simple tag scanner that allow you to list tags
and save them to a CSV.


## Installation

The preferred installation for `fort-tag-scanner` is to use
`snap`. `snap` comes pre-installed on recent Ubuntu distributions and
derivatives.

### Installation using `snap`

Prerequisite: if your distribution does not come with `snap`
pre-installed, please visit https://snapcraft.io/docs/installing-snapd

In a terminal, simply use the `snap install` command:

``` bash
sudo snap install fort-tag-scanner
```

`snap` application cannot have access to the computers cameras by
default, which is the purpose of `fort-tag-scanner`. You will need to
manually allow the `fort-tag-scanner` to access the system camera.

``` bash
sudo snap connections fort-tag-scanner:camera :camera
```

### Installation from sources

This method is reserved for developers. This project uses `cmake`.
You will need this additional dependencies:

* Qt 5
* OpenCV 4
* FORT myrmidon

## Getting started

`snap` will ghave automatically added the `FORT Tag Scanner` menu
entry in the system. Alternatively you can manually launch the
`fort-tag-scanner` command from a terminal.

Please refer to [Manual.md](manual.md) to refer on how to use the
application.

## Authors

The file [AUTHORS](AUTHORS) lists all copyright holders (physical or
moral person) for this repository.

See also the list of
[contributors](https://github.com/formicidae-tracker/studio/contributors)
who participated in this project.

## License

This project is licensed under the GNU Lesser General Public License
v3.0 or later - see the [COPYING.LESSER](COPYING.LESSER) file for
details



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[build-status-shield]: https://img.shields.io/travis/com/formicidae-tracker/tag-scanner/master?style=flat-square
[build-status-url]: https://travis-ci.com/formicidae-tracker/tag-scanner
[coverage-status-shield]: https://img.shields.io/coveralls/github/formicidae-tracker/tag-scanner?style=flat-square
[coverage-status-url]: https://coveralls.io/github/formicidae-tracker/tag-scanner
[contributors-shield]: https://img.shields.io/github/contributors/formicidae-tracker/tag-scanner.svg?style=flat-square
[contributors-url]: https://github.com/formicidae-tracker/tag-scanner/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/formicidae-tracker/tag-scanner.svg?style=flat-square
[forks-url]: https://github.com/formicidae-tracker/tag-scanner/network/members
[stars-shield]: https://img.shields.io/github/stars/formicidae-tracker/tag-scanner.svg?style=flat-square
[stars-url]: https://github.com/formicidae-tracker/tag-scanner/stargazers
[issues-shield]: https://img.shields.io/github/issues/formicidae-tracker/tag-scanner.svg?style=flat-square
[issues-url]: https://github.com/formicidae-tracker/tag-scanner/issues
[license-shield]: https://img.shields.io/github/license/formicidae-tracker/tag-scanner.svg?style=flat-square
[license-url]: https://github.com/formicidae-tracker/tag-scanner/blob/master/COPYING.LESSER
[product-screenshot]: images/screenshot.png
