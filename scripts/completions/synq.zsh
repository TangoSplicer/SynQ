# synq/scripts/completions/synq.zsh
# ZSH Completion for SynQ CLI and Scripts
# © 2025 SynQ Project

_synq_zsh_complete() {
  local -a commands
  commands=(
    create build run test plugin debug ai config
    mutate fuzz qtrace inspect skillmap chain sign
    version doctor init man
  )

  local -a scripts
  scripts=(${(f)"$(ls ${0:h:h}/scripts/*.sh | sed 's|.*/||')"})

  _describe 'command' commands
  _describe 'script' scripts
}

compdef _synq_zsh_complete synq