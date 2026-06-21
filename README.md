# Universal File Search Engine

A lightweight, fast, single-binary CLI tool for searching across heterogeneous file formats — logs, CSV, JSON, and plain text, with more formats on the way.

No servers, no setup, no dependencies to run. Built in modern C++17.

```bash
ufs search "ERROR" --file app.log
ufs search "Ali" --file users.csv
```

## Why

Most real-world systems produce data in mixed formats — structured logs, CSV exports, JSON Lines, config files. Searching across them usually means switching tools or writing one-off scripts per format.

Universal File Search Engine solves this with one core idea: **every file format is converted into a common `Record` structure before search happens.** The search engine, evaluator, and output layer never need to know whether a match came from a log line, a CSV row, or a JSON object.

## Status

Actively under development

### Implemented

- [x] Core `Record` model — unified representation across all formats
- [x] `IParser` interface + `ParserRegistry` for pluggable format support
- [x] `TextParser` — fallback, line-by-line search
- [x] `LogParser` — structured log parsing (timestamp, level, message)
- [x] `CsvParser` — header-based column extraction
- [x] `JsonParser` — JSON Lines format
- [x] `SearchEngine` — keyword search across any registered format
- [x] `ufs` CLI with subcommand structure (`ufs search ...`)
- [x] Unit tests with Catch2

### Planned (v1.0)

- [ ] `XmlParser`, `YamlParser`, `TomlParser`, `IniParser`
- [ ] Predicate query language: `level=ERROR AND responseTime>500`
- [ ] Multi-file and recursive directory search
- [ ] Result merging across files (priority queue, chronological order)
- [ ] Output formatters: table, JSON, plain
- [ ] Benchmark suite
- [ ] CI via GitHub Actions

### Planned (v2.0)

- [ ] Indexing for repeated queries on the same file
- [ ] Regex support in the query language
- [ ] Compressed file support (`.gz`)
- [ ] Plugin system for external parsers

## Architecture

Every parser converts its source format into a vector of `Record`:

```cpp
struct Record {
    std::unordered_map<std::string, std::string> fields;
    std::string            raw;
    std::size_t            lineNumber;
    std::filesystem::path  sourceFile;
};
```

Adding a new format means writing one class that implements `IParser` and registering it — nothing else in the codebase changes.

```cpp
class IParser {
public:
    virtual bool canParse(const std::filesystem::path& file) const = 0;
    virtual std::vector<Record> parse(const std::filesystem::path& file) const = 0;
    virtual std::string name() const = 0;
};
```

See [`docs/architecture.md`](docs/architecture.md) *(coming soon)* for the full design rationale.

## Building

Requires CMake 3.16+ and a C++17 compiler.

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Running tests

```bash
cmake .. -DBUILD_TESTS=ON
cmake --build .
ctest --output-on-failure
```

## Usage

```bash
ufs <command> [options]
```

### `ufs search`

```bash
ufs search <keyword> --file <path>
```

Example:

```bash
$ ufs search "ERROR" --file app.log
[app.log:1] 2026-06-20 14:32:11 ERROR connection timeout after 30s
[app.log:4] 2026-06-20 14:32:25 ERROR database connection refused

2 match(es) found.
```

Format is auto-detected from the file extension — no need to specify it.

More subcommands (`ufs index`, `ufs benchmark`, etc.) are planned as the project grows — see the roadmap above.

## Tech Stack

- C++17
- CMake
- [nlohmann/json](https://github.com/nlohmann/json) for JSON parsing
- [Catch2](https://github.com/catchorg/Catch2) for testing

## License

MIT