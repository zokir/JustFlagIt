#!/usr/bin/python3

from flagit import FlagIt

flag_obj = FlagIt('http://127.0.0.1:9000/wowza/sample.json')

assert flag_obj.enabled_for('feature1', 'john')
assert flag_obj.enabled_for('feature1', 'alex') is False
assert flag_obj.disabled_for('feature1', 'john') is False
assert flag_obj.disabled_for('feature1', 'alex') is False
assert flag_obj.disabled_for('feature1', 'kelly')
assert flag_obj.disabled_for('feature1', 'andrea')

assert flag_obj.enabled('feature1')
assert flag_obj.enabled('feature2') is False
print(flag_obj.enabled("feature1"))

try:
    print(flag_obj.enabled_for('feature2', 'alex'))
except RuntimeError as e:
    print(e)

try:
    FlagIt("http://127.0.0.1:9000/wowza/sample.json")
except ValueError as e:
    print(e)
