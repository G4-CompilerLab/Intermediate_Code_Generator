# Intermediate Code Studio — Compiler Design Lab

A simple, teacher-friendly web interface for an Intermediate Code Generator.

## Run the website

No installation or server is required.

1. Extract the project folder.
2. Open `index.html` in Google Chrome / Microsoft Edge.
3. Enter or paste a C-like source program in the editor.
4. Click **Generate Intermediate Code**.
5. Inspect Overview, Tokens, Symbol Table, Three Address Code, Quadruples and Triples.

## Example

```c
int a = 10;
int b = 5;
int c;
c = a + b * 2;
int result;
result = c - 3;
```

## Project contents

- `index.html` — website structure and UI
- `style.css` — complete visual theme
- `app.js` — browser-based lexical analysis, symbol table and intermediate-code generation
- `Final_intermediate_code_generator.c` — original C implementation supplied for the lab project/reference

## Technologies

HTML5, CSS3 and JavaScript. The website is static and can be deployed directly to GitHub Pages or hosted on a static web server.
