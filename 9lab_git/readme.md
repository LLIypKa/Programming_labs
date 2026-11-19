# на 3

1) git status - в результате мы увидели название текущей ветки lab9_git, в ней пока нет индексированых файлов (этот readme заполнялся на момент выполнения команды)
![alt text](images/git_status.png)
2) git log - текущий указатель HEAD и хэши коммитов с датой создания (на скрине последние коммиты)
![alt text](images/git_log.png)
4) git status - вывод не поменялся
![alt text](images/git_status_4.png)
6) git status - в выводе теперь все вновь добавленные (командой git add .) на момент выполнения файлы
![alt text](images/git_status_6.png)
7) git commit - при помощи флага -m добавлено сообщение для коммита
![alt text](images/git_commit.png)
8) git status - снова нет индексированных файлов
![alt text](images/git_status_8.png)
9) добавил документацию для метода 
![alt text](images/comment.png)
10) git status - отметил, какие файлы не проиндексированы и в каких индексированых есть изменения
![alt text](images/git_status_10.png)
11) git add . - проиндексировали все изменённые/добавленные файлы
12) git status - указывают все готовые к коммиту файлы
![alt text](images/git_status_12.png)
14) закоммитим изменения
![alt text](images/git_commit_14.png)
15) т.к. мы не проиндексировали новый коммент в файле + добавиили скринов и пунктов в readme, git status показывает непроиндексированные изменения/новые файлы. git log говорит, что теперь HEAD в последнем коммите
![alt text](images/git_status_15.png)
![alt text](images/git_log_15.png)
