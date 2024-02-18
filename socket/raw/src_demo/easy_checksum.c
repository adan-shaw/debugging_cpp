// 计算ICMP校验和
unsigned short checksum (void *buf, int len)
{
	unsigned short *data = buf;
	unsigned int sum = 0;

	while (len > 1)
	{
		sum += *data++;
		len -= 2;
	}

	if (len)
	{
		sum += *(unsigned char *) data;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	return ~sum;
}
