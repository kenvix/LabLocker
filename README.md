# 基于WLAN和TOTP的智能门禁控制系统

## 生成TOTP PSK
```shell
pip install pyotp
```
```python
import pyotp
pyotp.random_base32()
```