# на 3

1) git status - в результате мы увидели название текущей ветки lab9_git, в ней пока нет индексированых файлов (этот readme заполнялся на момент выполнения команды)
![alt text](images/3/git_status.png)
2) git log - текущий указатель HEAD и хэши коммитов с датой создания (на скрине последние коммиты)
![alt text](images/3/git_log.png)
4) git status - вывод не поменялся
![alt text](images/3/git_status_4.png)
6) git status - в выводе теперь все вновь добавленные (командой git add .) на момент выполнения файлы
![alt text](images/3/git_status_6.png)
7) git commit - при помощи флага -m добавлено сообщение для коммита
![alt text](images/3/git_commit.png)
8) git status - снова нет индексированных файлов
![alt text](images/3/git_status_8.png)
9) добавил документацию для метода 
![alt text](images/3/comment.png)
10) git status - отметил, какие файлы не проиндексированы и в каких индексированых есть изменения
![alt text](images/3/git_status_10.png)
11) git add . - проиндексировали все изменённые/добавленные файлы
12) git status - указывают все готовые к коммиту файлы
![alt text](images/3/git_status_12.png)
14) закоммитим изменения
![alt text](images/3/git_commit_14.png)
15) т.к. мы не проиндексировали новый коммент в файле + добавиили скринов и пунктов в readme, git status показывает непроиндексированные изменения/новые файлы. git log говорит, что теперь HEAD в последнем коммите
![alt text](images/3/git_status_15.png)
![alt text](images/3/git_log_15.png)
16) ![добавили всё в stage и закоммитили](images/3/git16.png)
17) git push - укажем в какой репозиторий и в какую ветку запушим изменения и увидем отчёт об операции git_push
![alt text](images/3/git17.png)
## ветки
1, 2) создадим ветку при помощи git branch и просмотрим список всех веток
![alt text](images/3/branch_1.png) 
3) переключимся при помощи git checkout на новую ветку
![alt text](images/3/branch_3.png)
4) git status при переключении на новую векту точно также покажет, какие файлы не проиндексированы. При переключениии на master без индексирования или без использования stash эти файлы перейдут в master и отобразятся как неиндексированные и в mater
![alt text](images/3/branch_4.png)
5) это можно сделать при помощи git branch
![alt text](images/3/branch_5.png)
7) git add . + git commit -m
![alt text](images/3/branch_7.png)
8) head указывает на новую ветку
![alt text](images/3/branch_8.png)
9) git checkout main
10) head указывает на последний коммит в этой ветке
![alt text](images/3/branch_9_10.png)
11) git add file1.txt + git commit -m
![alt text](images/3/branch_11.png)
12) теперь мы видим все ветвления репы
![alt text](images/3/branch_12.png)
13) git checkout mybranch
![alt text](images/3/branch_13.png)
14) да, пропал, т.к. он закоммичен в друой ветке 
15) видно разницув между текстовыми файлами, а также и их отсутствие в ветках mybranch/main
![alt text](images/3/branch_15.png)
16) ![alt text](images/3/branch_16.png)
17) ![alt text](images/3/branch_17.png)

# на 4
1) ![alt text](images/4/1.png)
2) ![alt text](images/4/2.png)
3) видно, были исправления в readme и sort.c
![alt text](images/4/3.png)
4) да, пустой
![alt text](images/4/4.png)
5) ![alt text](images/4/5.png)
6) он пуст, т.к. все измененя закоммичены
![alt text](images/4/6.png)
7) тоже пустой
![alt text](images/4/7.png)
8) ![alt text](images/4/8.png)
9, 10) git diff показывает все не индексированные изменения, а --staged пока пустой
11) git diff --staged показывает разницу между индексированными изменениями (HEAD vs cureent commit), и. т.к. последние изменения не индексированы, вывод у команды пустой. git diff просто показывает все изменения
12) единственное, два раза файл будет присутствовать в выводе, если проиндексировать изменения
![alt text](images/4/12.png)
13, 14) теперь видно, что просто существуют изменения в файле sort.c
![alt text](images/4/13_14.png)
15) ![alt text](images/4/15.png)
16) теперь head на последнем коммите
![alt text](images/4/16.png)
17) ![alt text](images/4/17.png)
18) ![alt text](images/4/18.png)
19) git status показывает, какие изменения могут быть проиндексированы
![alt text](images/4/19.png)
20, 21) теперь в sort.c последние закоммиченные изменения
![alt text](images/4/20_21.png)
22) теперь sort.c пропал из git status, т.к. в нем нет никаких изменений
![alt text](images/4/22.png)
23) ![alt text](images/4/23.png)
## ветки
1) ![alt text](images/4/branch1.png)
2) ![alt text](images/4/branch2.png)
3, 4) ![alt text](images/4/branch_3_4.png) 
5) указана новая ветка и непроиндексированные изменения
![alt text](images/4/branch5.png)
6) ![alt text](images/4/branch6.png)
7) ![alt text](images/4/branch7.png)
8) текущей указана feature/uppercase
![alt text](images/4/branch8.png)
9) видно структуру веток, также указатель HEAD указывает на текущую ветку, также видно, что ветка создана из mybranch
![alt text](images/4/branch9.png)
10) ![alt text](images/4/branch10.png)
11) ![alt text](images/4/branch11.png)
12) видно, что в файле лежит разный текст
![alt text](images/4/branch12.png)
13) ![alt text](images/4/branch13.png)
14) теперь в файле лежат изменения из feature/uppercase
![alt text](images/4/branch14.png)
15) ![alt text](images/4/branch15.png)
16) ![alt text](images/4/branch16.png)
17) виден граф от создания ветки mybranch до ее мерджа в main
![alt text](images/4/branch17.png)
1)  ![alt text](images/4/branch18.png)