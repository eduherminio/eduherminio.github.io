---
title: "Deprecating methods with optional arguments"
excerpt: "How to properly deprecate methods that have optional arguments."
date: 2020-12-05T00:00:00+00:00
categories:
  - blog
tags:
  - csharp
  - dotnet
  - library
  - versioning
---

## Introduction

### Optional arguments

From C# 4 methods, indexers, constructors, and delegates can have both [named and optional arguments](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/named-and-optional-arguments).

In the following example, `int bar` is a named argument and `bool isZeroIndexArray` is an optional argument, which becomes `true` if no other value is provided.

```csharp
void Foo(int bar, bool isZeroIndexArray = true) { }
```

### `ObsoleteAttribute`

Since .NET Framework 1.1 `ObsoleteAttribute` can be used to ["mark program elements as no longer in use"](https://docs.microsoft.com/en-us/dotnet/api/system.obsoleteattribute). Those program elements include classes, methods, fields, etc.

It's a common practice to place it in those elements that are no longer maintained and/or may disappear in future versions:

```csharp
[Obsolete("This method is obsolete. Please use xxxx instead.")]
void Foo(bool isZeroIndexArray = true) { }
```

### Semantic versioning

[Semantic versioning](https://semver.org/) is a common practice in software development nowadays (or at least it should be!).

Essentially, it's a way of versioning software that sticks to the following rules:

> Given a version number MAJOR.MINOR.PATCH, increment the:
>
> 1. MAJOR version when you make incompatible API changes,
> 2. MINOR version when you add functionality in a backwards compatible manner, and
> 3. PATCH version when you make backwards compatible bug fixes.

## Deprecating methods that have optional arguments

### Real life scenario

Sometimes one doesn't want their users to continue using a method/class/property because:

- There are plans to remove it in the future.
- There is already a working alternative in place that mimics its behavior.

Given that scenario, there are some chances that, sticking to semantic versioning and [its advice about handling deprecation](https://semver.org/#how-should-i-handle-deprecating-functionality), you want to release some minor versions where the legacy part of your code appears as 'obsolete' before actually removing it (and therefore bumping your major version).

And there are also some (smaller) chances that the method that needs to be deprecated contains [optional arguments](https://eduherminio.github.io/blog/deprecating-methods-with-optional-arguments/#optional-arguments).

That's the scenario we're going to focus on.

### Initial implementation and _gotcha_

Let's go back to one of the examples presented in the [introduction](https://eduherminio.github.io/blog/deprecating-methods-with-optional-arguments/#introduction):

We've always had the following method in our library:

```csharp
public class OurClass
{
    public static void Foo(bool isZeroIndexArray = true) { }

    // ...
}
```

We now want to allow our users to use `Foo()` method in a more flexible way, so we've implemented:

```csharp

public class FooConfiguration
{
    public bool IsZeroIndexArray { get; set; }

    public int Verbosity { get; set; }

    public FooConfiguration() { IsZeroIndexArray = true; }
}

public class OurClass
{
    public static void Foo(FooConfiguration configuration? = null) { }

    // ...
}
```

Since we don't want to duplicate or complicate our `Foo()` implementation, ideally we'd like to get rid of `void Foo(bool)`; but **we don't want to bump our major version** (yet).

However, **we do want to release our new code** with `void Foo(FooConfiguration?)`.

The solution, as we previously hinted, is marking the old one as obsolete, adding the new one and releasing the code with a new minor version. But **there's a _gotcha_** when doing that:

If we simply do

```csharp
public class OurClass
{
    [Obsolete("This method is obsolete. Please use Foo(FooConfiguration?) instead.")]
    public static void Foo(bool isZeroIndexArray = true) { }

    public static void Foo(FooConfiguration? configuration? = null) { }

    // ...
}
```

we will potentially break the following user's code:

```csharp
OurClass.Foo();
```

They'll get compiler errors similar to:

```bash
error CS0121: The call is ambiguous between the following methods or properties: 'OurClass.Foo(bool)' and 'OurClass.Foo(FooConfiguration?)
```

That is, we would be violating semantic versioning rules, so 💩.

### Correct implementation

Fortunately not all hope is gone and there's a way of keeping the backwards compatibility while releasing our new functionality:

```csharp
public class OurClass
{
    [Obsolete("This method is obsolete. Please use Foo(FooConfiguration?) instead.")]
    public static void Foo(bool isZeroIndexArray) { }

    public static void Foo(FooConfiguration? configuration = null) { }

    // ...
}
```

Did you notice the change? Now **our legacy `void Foo(bool isZeroIndexArray)` doesn't have its optional argument any more**.

This allows all possible usages of our method to keep compiling:

```csharp
// Will use Foo(bool FooConfiguration? = null)
OurClass.Foo();

// Will use Foo(bool isZeroIndexArray) and get a compiler warning
OurClass.Foo(true);

// Will use Foo(bool isZeroIndexArray) and get a compiler warning
OurClass.Foo(false);
```

And if we implement correctly `void Foo(FooConfiguration?)`, we'll manage to keep `Foo()`'s behavior consistent.
