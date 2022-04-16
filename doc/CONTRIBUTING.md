# Git workflow

Written to represent the expected structure of pull requests, including own ones. Also see [code style](./CODESTYLE.md).

## gitignore

To hide unnecessary environment details, no information about the tooling in use is committed to the repository. Therefore, the `.gitignore` file is absent, too.

## New features

A new feature *fooish bar* is created in a `feature/fooish-bar` branched from `dev`. If it somehow affects the repo philosophy, [readme](../README.md) is corrected accordingly. When both the implementation and tests are ready, the feature branch is squash-merged into `dev` (unless decided to postpone until a later release), described as "add *fooish bar*", and deleted - no junk commits are welcome in the final history.

## Merging into `master`

Once `dev` is ready to be shown, it is merged into `master`, creating a commit with a new version: `git log master --first-parent` is supposed to contain no "add something minor" noise.