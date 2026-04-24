# Event Driven Backtesting Engine for Options  - Python API Reference - AI GENERATED

This document provides a comprehensive guide to the Python API exposed by the C++ Backtesting Engine. The engine uses a high-performance event-driven architecture, allowing users to write complex option strategies in Python while leveraging C++ for execution and data handling.

---

## 1. Getting Started

A basic backtest setup involves defining a `Strategy`, configuring a `Backtester`, and adding data sources.

### Data Integration: External APIs
The engine supports fetching real-world US Options data automatically via the `URL` data source type.

**Requirements**:
1. Create a `config.json` in the project root with the following keys:
   ```json
   {
     "DATABENTO_API_KEY": "your_databento_key"
   }
   ```
2. Keys can be obtained from [Databento](https://databento.com/).

### Minimal Example: Bull Call Spread (fetching from APIs)
```python
import py_backtester as bt

class BullCallSpread(bt.Strategy):
    def on_event(self, event):
        if event.type == bt.EventType.MarketEvent:
            # Logic here...
            pass

# Execution using backtester
config = bt.BrokerConfig()
strategy = BullCallSpread()
engine = bt.Backtester(100000.0, strategy, config)

# Format: databento:TICKER:START_DATE:END_DATE
engine.add_data(bt.DataSourceType.URL, "databento:AAPL:2024-01-01:2024-02-01")
engine.run()

# OR if using CSV Format
engine.add_data(bt.DataSourceType.CSV, "path to your csv file")
engine.run() 
```

---

## 2. Core Classes

### `Backtester`
The main orchestrator of the simulation.

- **Constructor**: `Backtester(initial_capital: float, strat: Strategy, brokerConfig: BrokerConfig)`

- **`add_data(type: DataSourceType, source_string: str)`**: 
    - `DataSourceType.CSV`: `source_string` is the file path.
    - `DataSourceType.URL`: `source_string` is the API query string (`databento:TICKER:START:END`).

- **`run()`**: Starts the event loop.

- **`portfolio`** (Read-only property): Returns the `Portfolio` object.

### `Strategy`
Base class for all user-defined strategies.

- **Constructor**: `Strategy(name: str)`

- **`on_event(event: Event)`**: Virtual method. Overridden by the user to implement logic.

- **`push_signal(signal: SignalEvent)`**: Pushes a new signal to the execution queue.

- **`portfolio`** (Read-only property): Accesses the engine's `Portfolio`.

- **`datahandler`** (Read-only property): Accesses the engine's `DataHandler`.

### `OptionChain`
A snapshot of all available option contracts at a specific moment.

- **`date`**: The timestamp of the chain.

- **`get_underlying_price() -> float`**: Returns the spot price of the underlying.

- **`get_atm(type: OptionType, expiry: date) -> OptionContract`**: Finds the At-The-Money contract.

- **`get_contracts(type: OptionType, expiry: date) -> list[OptionContract]`**: Returns all contracts matching criteria.

- **`get_available_expiries() -> list[date]`**: Lists all unique expiration dates in the chain.

- **`find_by_symbol(symbol: str) -> OptionContract`**: Searches for a contract by its identifier.

### `OptionContract`
Represents a single option instrument.

- **Properties**:
    - `symbol`, `strike`, `type`, `expiration`, `multiplier`.
    - `marketdata`: Accesses OHLCV, Underlying Price, Risk-Free Rate, and Yield.
    - `greeks`: Accesses Delta, Gamma, etc.

- **Methods**:
    - `profit_at_expiration(price: float) -> float`: Calculates settlement PnL.

---

## 3. Event System

### `MarketEvent`
- **`chain`**: The `OptionChain` object for the current tick.

### `SignalEvent` (User Created)
- **Constructor**: `SignalEvent(timestamp, symbol, signal_type, quantity, order_mode=Market, price=0.0)`

### `FillEvent`
- **Properties**: `symbol`, `quantity`, `direction`, `fillprice`, `slippage`, `commision`.

---

## 4. Enums

### `DataSourceType`
- `CSV`: Load from local files.
- `URL`: Fetch from Databento.

### `SignalType`
- `Long`: Buy/Enter Long.
- `Short`: Sell/Enter Short.
- `Exit`: Close existing position.

### `OptionType`
- `Call`, `Put`

## 4. Pricing & Quantitative Tools

### `BlackScholesModel`
Allows manual calculation of Greeks and theoretical prices.
- **`calculate_price(option: OptionContract) -> float`**
- **`calculate_greeks(option: OptionContract)`**: populates the `greeks` property of the contract.
 