# Disclaimer

This repository is the product of two older repositories created for each project (labeled T1 and T2).

The way I managed to merge both repo in a single repo follows and is based on [this](https://www.youtube.com/watch?v=2JqWvl3HFfQ) video.

```bash
$ git clone <NEW_REPO>
$ cd <NEW_REPO>
-- FOR EVERY REPO LABELED <NAME> DO THE FOLLOWING --
$ git remote add <NAME> <URI>
$ git fetch <NAME>
$ git remote -v --> should show 2 remotes
$ git checkout -b <NAME> <NAME>/master
$ git branch --> should show both <NAME> and master
$ mkdir <FOLDER> --> where FOLDER will be the folder on <NEW_REPO>
$ git mv <FILES> <FOLDER>
$ git commit -m "Moved all files to subfolder <FOLDER>"
$ git checkout master
$ git merge <NAME> [--allow-unrelated-histories]
$ git commit -m "Merged with <NAME>"
$ git push
$ git remote rm <NAME>
$ git branch -d <NAME>
$ git branch -a -v should show only master
-- NOW YOU CAN DELETE REPO <NAME> --
-- REPEAT FOR EVERY REPO <NAME> AS WANTED --
```

