# 如何提交 PR

本文尽量使用命令行工具介绍如何向开源项目提交 **PR (Pull Request)**，一次亲身经历提交 **PR**！

## fork 项目

在浏览器打开 [https://github.com/jiejieTop/http-client](https://github.com/jiejieTop/http-client)，点击右上角的 `fork` 按钮，`fork` 开源项目到你自己的 `github` 账户。

![fork 项目](http://qiniu.jiejie01.top/how_to_pr001.png)

然后会自动进入你自己的仓库，开源看到相关的信息：

![fork 信息](http://qiniu.jiejie01.top/how_to_pr002.png)

接着你点击【Clone or download】按钮，会出来一个链接，点击②复制该链接（也可手动复制）。

## clone 项目

然后进入 linux 平台，打开终端，运行以下命令：

```bash
git clone [你复制的链接]
```

## 添加上游源仓库

添加上游源仓库是为了保证与原始仓库保持一致，因为原始仓库一直保持更新，使用以下命令添加上游源仓库:

```bash
git remote add upstream https://github.com/jiejieTop/http-client.git
```

添加完成后可以使用 `git remote -v` 查看当前的远程仓库地址信息:

```bash
git remote -v

origin  https://github.com/iot-strategy/http-client.git (fetch)
origin  https://github.com/iot-strategy/http-client.git (push)
upstream        https://github.com/jiejieTop/http-client.git (fetch)
upstream        https://github.com/jiejieTop/http-client.git (push)
```

## 更新源仓库

之后运行下面几条命令，就可以保持本地仓库和上游源仓库同步了，在每次在更新代码前需要运行一下以下的命令。

```bash
git fetch upstream
git checkout master
git merge upstream/master
```

## 修改代码，添加新功能

你可以选择切换分支开发也可以选择在 `master` 分支上进行开发，在开发完成后，可以提交到你自己的远端仓库。

**建议在自己的新分支上开发！！避免产生过多的冲突。**

接下来就是正常的 `git add`、`git commit`、`git push` 操作了，此处不做过多叙述。

## 提交 PR

当你提交上去后，可以看到 **【New pull request】** 按钮了，不要问，点它就对了。

![准备提交 PR](http://qiniu.jiejie01.top/how_to_pr003.png)

接着你看到以下的界面，看到 **【Create pull request】** 了吗，点它就对了。

![创建 PR](http://qiniu.jiejie01.top/how_to_pr004.png)

接着填写标题，描述一下你修改了哪里，最后点击 **【Create pull request】** 按钮，提交完成。

![填写信息](http://qiniu.jiejie01.top/how_to_pr005.png)

最后你会发现以下的界面，表示提交 PR 已完成，等待合并即可，在合并的时候 github 会通知你的。

![完成](http://qiniu.jiejie01.top/how_to_pr006.png)

**热烈欢迎大家参与到这个开源项目中来 []~(￣▽￣)~**

## 完
