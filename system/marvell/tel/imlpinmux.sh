
/system/bin/hwacc w 40e10294 000018c2
/system/bin/hwacc w 40e10298 000018c2
/system/bin/hwacc w 40e1029c 000018c2
/system/bin/hwacc w 40e102a0 000018c2
/system/bin/hwacc w 40e102a4 000018c2
/system/bin/hwacc w 40e102a8 000018c2

echo "									"
echo "	IML pin-mux setup was done		"
echo "									"
echo -0x28 > /proc/driver/88pm860x
echo "PMIC enable MMC LDO:"
echo 0x0b > /proc/driver/88pm860x
echo "									"
echo "	IML voltage (MMC) was enabled	"
echo "									"

