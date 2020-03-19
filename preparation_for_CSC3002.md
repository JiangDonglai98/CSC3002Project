由于我不清楚你们每个人目前对语言和一些工具的掌握情况，所以我这里从头开始说。

### C++

#### 基础

首先建议速刷 C++ Primer （不是 C++ Primer Plus）这本书。

希望能达到的目标：

- 对 C++ 的标准库容器有着“知道有这么一个东西&什么情况下应该用它”的基本认识

- 对 C++ 的整套 OOP 系统有着基本的认识

- 能够写简单的模板

#### 查阅资料

建议使用 [cppreference](https://en.cppreference.com/w/Main_Page)

中文有一些小问题，但是总体而言质量还行。觉得英文阅读速度太慢的也可以看中文页面。

cppreference 本身自带的搜索很垃圾，建议使用 Google 来搜索。

举例：`value category site:cppreference.com` 或者 `value category cppreference`

也非常欢迎直接在群里提问。

#### 规范

见 `STYLE_GUIDE.md`。有疑问的请尽早在群里提出进行讨论，在开始项目之前都可以改。

#### 其他

所有的问题建议在群里提问，只问我的话可能得到的答案并不是最好的。

对于搭建工作环境有困扰的，我也可以尽量帮忙。

如果对自己 C++ 水平不是非常自信的话，建议把我和胡海川加为 collaborator，在提交 pr 的时候 at 我们来进行 code review。另外如果要进行 code review 的话，请在开发的一开始就提交 pr，并且在 pr 的标题后面加上 `[WIP]` （Work in Progress），然后 at 我和胡海川。每次 commit 尽量限制在 50 行代码以内，方面 review 和修改，以及及时制止某些错误。分支开发完成后将标题的 `[WIP]` 去掉。如果我们都 prove 了那么就可以 merge 了。

第三方库方面我正在考虑。欢迎自己选择顺手的第三方库来使用，只要所属项目内所有人意见一致。

### Git

建议至少看完 https://git-scm.com/book/zh/v2 的 1 2 3 5 6 章节。

除了纯命令行的 git 以外，也可以尝试编辑器/IDE内自带的 git 功能，或者 github desktop，或者像 sourcetree 这样的工具。这些就靠你们自己去摸索了。

我们采用 collaborator 的形式进行协作，每个人都有权限读写，所以请直接在项目上开出自己的工作分支，然后提交 pr。另外不再设立 develop 分支，直接从特性分支向 master 进行 merge。慎用 rebase。

Git commit 规范见 `STYLE_GUIDE.md`。

### Github

建议在 https://lab.github.com/ 根据自己的需要有选择地进行学习。

希望能达到的目标：

- 掌握 repo 相关的基本操作（如开分支、提交 pr 等）

- 掌握 Github Flavored Markdown 的基本语法

- 掌握 issue、pr 等功能的使用

Github Actions 是一个可选的学习内容。不学也没关系，我会尽量协助搭建。但是我本人也是从零开始学习。

### CMake

这也是一个可选的学习内容，但是每个项目内至少应该有一个人会用。

可以参考[这篇回答](https://www.zhihu.com/question/58949190/answer/999701073)进行学习。

### 测试

我有打算使用 Google Test 这个框架进行单元测试，它看起来学习成本并不高。

我希望每个人负责自己编写部分的单元测试，而不是有专门的测试人员。有两个原因：

1. 负责测试的同学评分可能较低。

2. 很可能会出现负责测试的同学因为缺少相关知识而无法写出测试的情况。

对于过于简单的部分可以不编写测试，目前来看效率比较重要。

### 已定的人员分配

#### Compiler

- 何伊亮

- 胡海川

#### Assembler & Simulator

- 张昕熠

#### Editor

- 无

因为我不清楚其他人擅长的或者感兴趣的点，所以希望你们自己之间讨论作出选择了。

另外提前完成所负责 part 的同学欢迎来帮忙其他 part 。

最后，请尽快对自己所属的项目有一个大致的了解，并且定下合理的最终期望，以便期中提交 proposal。
