# Porsche Touch UI Reference Notes

These notes summarize the screenshot set used for the fourth left-menu test page in `data/touch.html`.

Local comparison captures are stored in `documentation/touch-ui-reference/screenshots/`.

## Layout

- Fixed wide display composition, matching a 1024x600 touchscreen target.
- Persistent left icon rail with stacked app icons and a compact active state.
- Horizontal top tab bar with four large labels. The selected tab uses cyan text, a cyan underline, and a subtle dark-blue fill.
- Main content is split into a left information/control region and a right visualization region.
- A square time area sits at the top right beside the tabs.

## Visual Language

- Background is pure black with very restrained glow and dot-grid depth behind the vehicle image.
- Dividers are thin, low-contrast white/gray rules. Rows are separated by horizontal rules instead of card containers.
- Accent color is cyan/ice blue. It appears in active tab lines, selected rows, switch knobs, radio fills, and climate arcs.
- Typography is tall, light, and condensed-feeling. Large labels are used in rows; secondary values are smaller and gray-white.
- Controls are compact: radio circles, pill toggles, square checkboxes, overlay menus, and small square action buttons.
- Checkboxes are small square marks. They keep a thin white outer frame; the active state adds a cyan inner fill with a narrow black gap between fill and frame.

## UI Elements Replicated

- Drive list rows with icons, primary labels, secondary values, and selected-row cyan bands.
- Option overlay panel with radio-button choices.
- Comfort/assistance list with back-title row and toggle switches.
- Trip summary list with gray section headers and numeric metric rows.
- Climate screen with mode labels, airflow/vehicle visualization, temperature controls, auto fan controls, seat icons, and bottom strip.
- Vehicle visualization area with car silhouette, green battery module, suspension arrows, airflow lines, and dotted floor texture.
- Top-right time square next to the tabs.

## Implementation Boundary

The current production touch UI is not replaced. The Porsche-style recreation is isolated behind the fourth left-nav entry (`styleTest`) and uses static mock content for design validation.
