# Write myfile.iso to a USB drive
cp myfile.iso /dev/sdb

# Rip a cdrom to a .iso file
cat /dev/cdrom > myfile.iso

# Create a gzipped image
gzip -9 < /dev/sdb > /tmp/myimage.gz
