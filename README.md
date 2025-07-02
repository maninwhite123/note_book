# note_book
一.Git库管理说明
1.从GitHUb上新建仓库
2.从GitHub上下载仓库    git clone git@github.com:MmlSQ/gitTest.git
3.上传代码到GitHub本地仓库
    查看仓库状态    git status
    新建的 "***" 文件处于 "Untracked" 状态。
    将文件添加到暂存区  git add .
    已成功将 "***" 添加到了暂存区，文件状态处于 "Staged"
    将暂存区文件提交到本地版本库中  git commit -m "gitTset first file"
    成功将 "***" 提交到了本地版本库中，文件状态处于 "committed"。
4.上传文件到远程仓库
    查看远程仓库的名字  git remote
    将文件提交到远程仓库指令    git push origin main
    命令 "git push 远程仓库名 分支"，可以看到远程仓库名为 "origin" ，分支为 "main"，因为当前仓库只有一个分支，而且名字是 "main"，可以从命令行的最后看出，也可以通过使用 "git branch" 命令查看。
5.手动建立与远程仓库的关联
    在桌面创建文件夹，在文件夹中打开Git Bash，最后初始化Git本地仓库。
    关联 "gitws" 本地仓库与Github的 "gitTest" 远程仓库，使用如下命令：  git remote add gittest git@github.com:MmlSQ/gitTest.git
    命令中的 "gittest" 是我们给远程仓库自定义的名字，而后面跟的是远程仓库的地址。
    关联过后可以使用git remote指令查看远程仓库的名字。
    使用下面命令，将远程仓库内容同步到本地仓库  git pull gittest HEAD
    命令 "git pull 远程仓库名 本地分支" 是拉取远程仓库的文件到本地仓库的指定分支里，因为HEAD指定的是本地仓库当前所选分支。
6.修改文件并同时同步到本地仓库和远程仓库
    在 "gitws" 本地仓库中修改README.md的内容。
    将已修改的内容添加到暂存区，并提交到本地版本库  git add . && git commit -m "modify README.md"
    最后上传到远程仓库  git push gittest master
    上传成功，这时，我们去康康远程仓库的内容，诶，等等，怎么不对呢，明明有提示上传，怎么没有修改呢？
    点开 "main" 旁边的箭头，发现多了一个 "master" 分支，原来我们刚才在本地仓库使用的是master分支，上传远程仓库时，远程仓库原本只有main分支，为了使远程与本地仓库分支同步，就又新建了一个master分支。
    在Git Bash中输入以下命令，创建并切换到main分支：    git checkout -b main
    再执行合并分支命令，表示把master分支合并到main主分支上：    git merge master
    合并完成，假设我们在本地仓库和远程仓库都不想要master分支，只想保留一个main分支。    删除本地分支命令：  git branch -d master
    删除远程仓库分支命令：  git push gittest --delete master    命令中的 "gittest" 是远程仓库名，master是要删除的分支。
    此时再重新上传远程仓库，使用以下命令：  git push gittest main

