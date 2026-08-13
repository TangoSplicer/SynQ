"""Read-only metadata for illustrative SynQ source examples.

The catalog intentionally exposes only descriptions and identifiers. It does not
compile, execute, or validate the historical example source material.
"""

from dataclasses import asdict, dataclass
from typing import Iterable


@dataclass(frozen=True)
class ExampleSummary:
    """A source-only example that has not been verified as executable."""

    id: str
    domain: str
    title: str
    description: str
    verification: str = "source-only-unverified"


_EXAMPLES: tuple[ExampleSummary, ...] = (
    ExampleSummary(
        id="quantum-vqe",
        domain="quantum",
        title="VQE for H2 Molecule",
        description="Illustrative source for a variational quantum eigensolver workflow.",
    ),
    ExampleSummary(
        id="machine-learning-quantum-neural-network",
        domain="machine-learning",
        title="Quantum Neural Network",
        description="Illustrative source for a hybrid quantum-classical neural-network workflow.",
    ),
    ExampleSummary(
        id="data-science-dataframe",
        domain="data-science",
        title="DataFrame Analysis",
        description="Illustrative source for a data-frame analysis workflow.",
    ),
    ExampleSummary(
        id="systems-file-operations",
        domain="systems",
        title="File Operations",
        description="Illustrative source for filesystem operations.",
    ),
)


def available_domains() -> set[str]:
    """Return catalog domains supported by the read-only recovery route."""
    return {example.domain for example in _EXAMPLES}


def list_examples(domain: str | None = None) -> list[dict[str, str]]:
    """Return all catalog entries, optionally constrained to one known domain."""
    return [asdict(example) for example in _EXAMPLES if domain is None or example.domain == domain]


def find_example(example_id: str) -> dict[str, str] | None:
    """Return one catalog entry by stable identifier, if it exists."""
    return next((asdict(example) for example in _EXAMPLES if example.id == example_id), None)
