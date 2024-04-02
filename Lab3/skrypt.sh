#!/bin/bash

# Funkcja do obsługi równoległego uruchamiania poleceń
run_command() {
    PID=$!
    echo "Uruchomiono proces o PID: $PID z poleceniem: $@"
    "$@"
    echo "Zakończono proces o PID: $PID z poleceniem: $@"
}

COMMANDS=("sleep 10" "sleep 3" "sleep 5")
PIDS=()

for command in "${COMMANDS[@]}"; do
    run_command $command &
    PIDS+=($!)
done

for pid in "${PIDS[@]}"; do
    wait $pid
done

echo "Wszystkie procesy zakończyły działanie."
