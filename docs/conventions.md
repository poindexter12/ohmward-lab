# Conventions

- All new hardware builds live under `projects/<project-name>/`.
- Each project should have:
  - `README.md`
  - `docs/` with wiring and BOM
  - `hardware/` with schematics and enclosures
  - `firmware/` with code or ESPHome configs

- ESPHome WiFi credentials must come from `secrets.yaml` and should not be
  committed.
