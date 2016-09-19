---
short-description: Source code version control with git
...

# Version Control

git is used for version control for all GNOME projects. This page
assumes good working knowledge of git; some introductory material is
available [here](https://www.atlassian.com/git/tutorials/), and a [git
cheatsheet is
here](https://training.github.com/kit/downloads/github-git-cheat-sheet.pdf).

  - Make atomic, revertable commits.
    ([](#guidelines-for-making-commits))

  - Include full reasoning in commit messages, plus links to relevant
    bug reports or specifications. ([](#guidelines-for-making-commits))

  - Keep large changes, such as renames, in separate commits.
    ([](#guidelines-for-making-commits))

  - Merge changes from feature branches by rebasing. ([](#use-of-git))

## Use of Git

Most GNOME repositories follow these rules:

  - No forced pushes. Except for branches with the `wip/` prefix
    (work-in-progress), the commits’ history must not be modified, as
    contributors rely on it.

  - Rebase commits rather than merging, to have a linear history (which
    is easier to follow).

  - Work on feature branches on GNOME git in `wip/` branches, then
    rebase on master and fast-forward merge the changes. It is a good
    practice to also add your nickname to the branch name, as
    `wip/nickname/feature`.

  - Hide [sausage
    making](https://sethrobertson.github.io/GitBestPractices/#sausage)
    by squashing commits before merging.

## Guidelines for Making Commits

Commits should be as small as possible, but no smaller. Each commit
should address a single issue, containing only changes related to that
issue. The message for each commit should describe the issue, explain
what causes it, and explain how it has been fixed if it is not obvious.
If the commit is associated with a bug report, the full URI for the bug
report should be put on a line by itself at the bottom of the commit
message. Similarly, the ID for the git commit (from **git log
--oneline**) should be copied into the bug report once the commit has
been pushed, so it is easy to find one from the other.

The changes in each commit should be easy to read. For example, they
should not unnecessarily change whitespace or indentation. Large,
mechanical changes, such as renaming a file or function, should be put
in separate commits from modifications to code inside that file or
function, so that the latter changes do not get buried and lost in the
former.

The following principles give the reasoning for all the advice above:

  - Each commit should take the repository from one working state to
    another, otherwise
    [bisection](http://git-scm.com/book/en/v2/Git-Tools-Debugging-with-Git#Binary-Search)
    is impossible.

  - Each commit should be individually revertable. If it later turns out
    that the commit was a bad idea, **git revert commit ID** should take
    the repository from a working state to another working state.

  - The reasoning for each commit, and its relationship to external
    resources like specifications and bug reports, should be clear, to
    the extent that commits written by one developer a year in the past
    should still be understandable by a second developer without having
    to trace through the changes and work out what they do.

  - Each commit should be written once, and designed to be read many
    times, by many reviewers and future programmers.

## Merging Procedure

To merge a feature branch named `my-branch` into master, use the
following commands:

```git
git checkout master
git pull

git checkout wip/
git rebase --interactive master
# Ensure the rebase is successful; test the changes

git checkout master
git merge wip/
git push

# wip/ can now be deleted
git push origin :wip/
git branch -D wip/
```

## External Links

  - [Git best
    practices](https://sethrobertson.github.io/GitBestPractices/)

  - [Git FAQ](https://help.github.com/categories/using-git/)

  - [Atlassian git tutorial](https://www.atlassian.com/git/tutorials/)

  - [Official git tutorial](http://git-scm.com/docs/gittutorial)

  - [Interactive git tutorial](https://try.github.io/)

  - [git-tower tutorial](http://www.git-tower.com/learn/)
