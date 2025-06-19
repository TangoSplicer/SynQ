# SynQ Webhook Plugin

Sends HTTP webhook notifications during plugin execution. Useful for:

- CI/CD triggers
- Observability pipelines
- Team alerts

## Configuration

- `url`: Target webhook URL
- `event`: One of `"job_success"`, `"plugin_installed"`, `"run_complete"` etc.

## Usage

```bash
synq run synq_webhook.synq --config url=https://api.myci.com/hook event=job_success