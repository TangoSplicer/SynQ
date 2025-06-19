# synq/scripts/completions/synq.bash
# Bash Completion for SynQ CLI and Scripts
# © 2025 SynQ Project

_synq_completions() {
  local cur prev opts corecmds advcmds meta scripts

  COMPREPLY=()
  cur="${COMP_WORDS[COMP_CWORD]}"
  prev="${COMP_WORDS[COMP_CWORD-1]}"

  corecmds="create build run test plugin debug ai config"
  advcmds="mutate fuzz qtrace inspect skillmap chain sign"
  meta="version doctor init man"
  scripts=$(cd "$(dirname "${BASH_SOURCE[0]}")/../" && ls scripts/*.sh 2>/dev/null | sed 's|scripts/||')

  opts="$corecmds $advcmds $meta $scripts"

  COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
  return 0
}

complete -F _synq_completions synq