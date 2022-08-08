# FlagIt 
Multi language feature flags framework.

Say you have a json describing a feature:

```
{
    "feature1": {
        "enabled": true,
        "enabled_for" : ["robert", "john"],
        "disabled_for" : ["andrea", "kelly"]
    }
}
```

Lets say you upload above json to random http service at location:

https://www.example.com/hello/my-lovely-flags

Now you can:

**Python**

```
    from flagit import FlagIt

    flag_obj = FlagIt('https://www.example.com/hello/my-lovely-flags')

    # Default value
    assert flag_obj.enabled('feature1')
    assert flag_obj.enabled('feature2') is False

    # Inclusion list
    assert flag_obj.enabled_for('feature1', 'john')
    assert flag_obj.enabled_for('feature1', 'alex') is False

    # Exclusion list
    assert flag_obj.disabled_for('feature1', 'john') is False
    assert flag_obj.disabled_for('feature1', 'alex') is False
    assert flag_obj.disabled_for('feature1', 'kelly')
    assert flag_obj.disabled_for('feature1', 'andrea')

```

**C++**
```
    flagit::FlagIt flags("https://www.example.com/hello/my-lovely-flags");

    # Default value
    REQUIRE(flags.enabled("feature1"));
    REQUIRE_FALSE(flags.enabled("feature2"));

    # Inclusion list
    REQUIRE(flags.enabledFor("feature1", "john"));
    REQUIRE_FALSE(flags.enabledFor("feature1", "alex"));

    # Exclusion list
    REQUIRE_FALSE(flags.disabledFor("feature1", "john"));
    REQUIRE_FALSE(flags.disabledFor("feature1", "alex"));
    REQUIRE(flags.disabledFor("feature1", "kelly"));
    REQUIRE(flags.disabledFor("feature1", "andrea"));
```

**Java**
In progres..
