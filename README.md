# 🌊 GitFlow TUI

A blazing fast, mobile-first, power-user terminal user interface (TUI) for GitHub. Built entirely in Go, GitFlow sits on top of the standard GitHub CLI (`gh`) to provide a beautiful, asynchronous, and deeply integrated dashboard for your repositories. 

Designed specifically to run flawlessly in environments like Termux on Android, as well as standard desktop terminals.

## ✨ Core Features

- **Multi-Tab Dashboard:** Seamlessly navigate between Pull Requests, Issues, CI/CD Pipelines, Local Files, and your GitHub Notifications Inbox.
- **The Workspace Sync:** Press `S` to unleash a concurrent fleet of Go workers that recursively finds every `.git` repository in your configured workspace and pulls the latest changes in the background. Features a granular failure-reporting modal.
- **The Review Engine:** Perform inline code reviews directly from the terminal. Jump into a PR's diff, press `r` or `enter` on any file, and submit feedback without ever opening a browser.
- **Native Interactions:** Read rich Markdown, submit general comments, and create new Pull Requests or Issues entirely within the TUI using native floating forms.
- **The Ghost Handoff:** Press `t` on any Pull Request to instantly stash your current work, check out the PR branch to test the code locally, and perfectly restore your original environment the moment you exit.
- **CI/CD Management:** Monitor GitHub Actions in real-time. View logs, cancel stuck runs, or trigger reruns with single keystrokes.
- **Dynamic Settings:** A built-in configuration editor allows you to completely remap the application's primary and border colors, default tab, and workspace root directory on the fly.
- **Fuzzy Filtering:** Type `/` to instantly filter hundreds of PRs, files, or notifications locally without waiting for API calls.

## 🚀 Installation

### Prerequisites

1. **Go:** (1.21 or higher recommended)
2. **GitHub CLI (`gh`):** Installed and authenticated (`gh auth login`)
3. **Git:** Installed and configured

### Building from Source

```bash
git clone [https://github.com/TangoSplicer/gitflow-tui.git](https://github.com/TangoSplicer/gitflow-tui.git)
cd gitflow-tui
go mod tidy
go build -ldflags="-s -w" -o gitflow .
```

Move the binary to your system path (e.g., in Termux):

```bash
mv gitflow $PREFIX/bin/
```

## 📖 Comprehensive User Guide

GitFlow operates primarily via single-keystroke commands. Press `?` anywhere in the app to open the Help Overlay.

### Global Commands (Available Anywhere)
* `tab` / `l`: Next tab
* `shift+tab` / `h`: Previous tab
* `/`: Fuzzy filter the current active list
* `y`: Copy the highlighted item's URL or local path to your clipboard
* `+` / `C`: Open the PR / Issue creation form
* `S`: Trigger the **Workspace Sync** (pulls all repos in your configured root)
* `,`: Open the Settings Menu (Configures colors, default tab, and sync root)
* `?`: Toggle the Help Menu
* `q`: Quit the application

### Forms & Commenting
* `Ctrl+D` or `Ctrl+S`: Submit the current form or comment (Ctrl+D recommended for Termux users)
* `Esc`: Cancel / Abort and return to the previous screen

### Pull Requests & Issues Tab
* `r`: Reply (Opens the commenting engine for the selected PR/Issue)
* `t`: **Ghost Handoff** (Stashes local work, checks out the PR branch, opens a sub-shell, and restores everything when you type `exit`)
* `v`: View changed files (Opens the File Viewer diff for the selected PR)
* `m`: Merge the Pull Request
* `c`: Close the Pull Request or Issue
* `enter` / `o`: Open the PR/Issue in your device's web browser

### File Viewer (Inside a PR)
* `enter` / `r`: Open the **Review Engine** to leave a comment on a specific file
* `e`: Edit the file locally (uses your `$EDITOR` or `nano`)

### CI/CD Pipelines
* `v`: View the full execution logs for the selected workflow run
* `w`: Rerun the workflow
* `x`: Cancel the running workflow

### Inbox (Notifications)
* `e`: Mark the notification as read
* `enter` / `o`: Open the notification target in your device's web browser

## 🙏 Acknowledgments & Technologies

This project stands on the shoulders of giants. A massive thank you to the open-source community, specifically:

* **[Charmbracelet](https://charm.sh/):** The absolute pioneers of modern CLI tooling (`bubbletea`, `lipgloss`, `bubbles`, `huh`, `glamour`).
* **[Atotto Clipboard](https://github.com/atotto/clipboard):** For handling seamless, cross-platform clipboard copy/pasting.
* **[GitHub CLI](https://cli.github.com/):** For providing the robust, secure authentication and API layer that powers our data fetching.

---
*Built for the Power User Era.*
