#!/usr/bin/python3

from flagit import FlagIt

flag_obj = FlagIt('http://127.0.0.1:9000/wowza/sample.json')
feature_value = flag_obj.value('feature1')
users = feature_value.as_list()
print(users)
assert ('robert', 'john') == users
print(flag_obj.enabled_for('feature1', 'john'))
print(flag_obj.enabled_for('feature1', 'alex'))

try:
    print(flag_obj.enabled_for('feature2', 'alex'))
except RuntimeError as e:
    print(e)

