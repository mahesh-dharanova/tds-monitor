import gzip, sys
src, dst = sys.argv[1], sys.argv[2]
data = gzip.compress(open(src,'rb').read(), compresslevel=9, mtime=0)
rows = []
for i in range(0, len(data), 16):
    rows.append('  ' + ','.join('0x%02x' % b for b in data[i:i+16]))
open(dst,'w').write(
"""// Generated from dashboard.html. Do not edit by hand.
// Regenerate with: python3 make_gz_header.py dashboard.html dashboard_gz.h
#pragma once
#include <stdint.h>
#include <stddef.h>

const size_t DASHBOARD_GZ_LEN = %d;
const uint8_t DASHBOARD_GZ[] = {
%s
};
""" % (len(data), ',\n'.join(rows)))
print('gz bytes:', len(data), 'from', len(open(src,'rb').read()))
