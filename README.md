# VSS-SampleCpp [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)][gpl3]

[![Trello](https://img.shields.io/badge/Trello-SDK-blue.svg)][vss-sdk-trello]
[![Slack](https://img.shields.io/badge/Slack-Channel-551a8b.svg)][slack]

**Branches**
1. receive-states
2. send-commands
3. send-debugs
4. all-communications
5. with-stdin-options
6. object-oriented
7. unit-tests

O VSS-SampleCpp são exemplos de códigos em C++ que se comunicam com os projetos do VSS-SDK.
Esse projeto utiliza o [VSS-Core][vss-core] como interface de comunicação.

Mais informações podem ser encontradas em [VSS-SDK][vss-sdk].

## Desenvolvimento
```
$ sudo ./configure.sh
$ cd build
$ ./vss-sample
```

# Licença

Esse código está sob licença [GNU GENERAL PUBLIC LICENSE Version 3][gpl3], cujo uma cópia em texto pode ser encontrada em [LICENSE.txt](LICENSE.txt).

Você pode utilizar esse código. Caso o faça, nos informe.

[gpl3]: http://www.gnu.org/licenses/gpl-3.0/
[travis]: https://travis-ci.org/SIRLab/VSS-SampleRust
[install]: https://github.com/SIRLab/VSS-SampleCppy/blob/master/INSTALL.md
[vss-sdk]: http://sirlab.github.io/VSS-SDK
[vss-core]: https://github.com/SIRLab/VSS-Core
[vss-sdk-trello]: https://trello.com/b/b4dVV6ug/vss-sdk
[slack]: https://vss-sdk.slack.com