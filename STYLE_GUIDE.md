# Style Guide for This CSC3002 Group Project

## Code Style

See [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
(C++17 version), except followings:

- Use `.cpp/.hpp` instead of `.cc/.h`.

- Function names should be in the same pattern as normal variable names.

- **Exceptions are allowed when necessary** (but not encouraged).

- For detailed formatting style, please refer to the `.clang-format` file of
  this project, which is based on clang-format 9.

- For detailed comment style, see below.

## Comment Style

See http://www.doxygen.nl/manual/docblocks.html. Note that:

- Use `//` or `/* */` for non-document comments.

- Use JavaDoc style for multi-line documents.

- Use `/** */` for one-line documents.

- Do not use banners.

- Do not omit `@brief`.

- Only mark `[out]` parameters.

Some examples:

```cpp
/**
 * @brief Balabalabalabalabalabalabalabalabalabalabalabalabalabalabalabalabalaba
 *        labala.
 * @tparam T Balabala
 * @param foo Balabala
 * @param[out] bar Balabala
 * @return Balabala
 */
template<typename T>
bool func(T foo, T* bar);

/** @brief Balabalabala. */
bool comp(int a, int b);

/**
 * @brief Balabalabalabalabalabalabalabalabalabalabalabalabalabalabalabalabalaba
 *        labala.
 */
class Foo {};
```

## Commit Style

See https://github.com/RomuloOliveira/commit-messages-guide.
