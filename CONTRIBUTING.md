# Contributing

Right now the areas that need the most help are:
- Union types
- Generics

# Building

There's a Makefile for the project, so compilation _should_ be easy. There's two targets right now,
native and JS. Native is mostly used for testing and not distributed any where. The JS target is what
is distributed on NPM and is interfaced by `yargs`.

To build for native:

```
$ make native
```

The native target is great for testing because it compiles super fast! However, it does not run `refmt`
on the results so be wary of syntax errors. To run the native binary, use:

```
$ ./cli.native -debug _test.js
```

This will print the stdout what ReasonablyTyped saw as the Flow definition and print
the result of compiling.

To build the JS target, run:

```
$ make js
```

The JS target runs `refmt` against all output, and is what is distributed through npm. Make
sure you test with this target before shipping a release!

# How is the code structured?

The main compiler is written is Reason, `compile` being defined in `src/retyped.re`. `src/retyped_node.re`
is compiled with JSOO and exposes `compile` to the JS target. `retyped_node.js` and `refmt_node.js` are
both interfaced by `index.js`. `index.js` exposes
a function for compiling called `compile` (different than the one exported from Reason) that
formats the code and handles errors cleanly. `cli.js` is the main entry for the `retyped` CLI tool, which
calls `index.js`.

The compiler is split into three parts, the module-definition generator, the code generator, and the
renderer. The module-definition generator is found in `src/modulegen.re` and the code generator is found in
`src/codegen.re`. The module-definition generator extracts an AST-like object from a Flow AST. This
is then passed to the code generator, which spits out Reason code as a string. The functionality from
both is glued together in `src/retyped.re`. Code snippets themselves are generated from `src/render.re`.

```
*-------------*     *------------------*     *----------------*
| Flow parser | --> | Module Generator | --> | Code Generator |
*-------------*     *------------------*     *----------------*
                                                     /\
                                                     ||
                                                     \/
                                                *----------*
                                                | Renderer |
                                                *----------*
```

# Testing

To run tests:

```
$ npm test
```

Make sure you build the JS files first!

ReasonablyTyped uses Jest for testing. The main test suite can be found in `test`. Currently, the test
suite looks at all the `.js` files in `test/fixtures`, compiles them, and compares the result to a
corresponding `.re` file, as well as several snapshot tests.

There is also a test suite against all FlowTyped definitions. Run this with:

```
$ npm run test:flow-typed
```

A lot of these tests will fail, but that's OK because the project is still in development.
When that test turns green we'll be ready for production :smiley:
