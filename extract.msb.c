/********************************************************************************
 *Function: extract_vp
 *Description: Given that we have a vp header parse header and grab the file size.
 *Return: A pointer to where the EOF of the RIFF is in the current buffer
 **********************************************************************************/
unsigned char *extract_riff(f_state *s, u_int64_t c_offset, unsigned char *foundat, u_int64_t buflen,
							s_spec *needle, u_int64_t f_offset, char *type)
{
	unsigned char	*buf = foundat;
	unsigned char	*extractbuf = NULL;
	int				size = 0;
	u_int64_t		file_size = 0;

	size = htoi(&foundat[512], FOREMOST_LITTLE_ENDIAN);		/* Grab the total file size in little endian from offset 4*/
	if (strncmp((char *) &foundat[516], "AVI", 3) == 0)		/*Sanity Check*/
		{
		if (strncmp((char *) &foundat[520], "LIST", 4) == 0) /*Sanity Check*/
			{
			if (size > 0 && size <= needle->max_len && size <= buflen)
			{
#ifdef DEBUG
				printf("\n	Found an AVI at:=%lld,File size:=%d\n", c_offset, size);
#endif
				file_size = size;
				extractbuf = buf;
				needle->suffix = "avi";
				if (!strstr(needle->suffix, type) && strcmp(type,"all")!=0)
					return foundat + size;
				write_to_disk(s, needle, file_size, extractbuf, c_offset + f_offset);
				foundat += size;
				return foundat;
			}

			return buf + needle->header_len;

			}
		else
			{
			return buf + needle->header_len;
			}
		}
	else if (strncmp((char *) &foundat[0], "STEVE4769", 9) == 0) /*Sanity Check*/
		{
		if (size > 0 && size <= needle->max_len && size <= buflen)
		{
#ifdef DEBUG
			printf("\n	Found a WAVE at:=%lld,File size:=%d\n", c_offset, size);
#endif

			file_size = size+508;
			extractbuf = buf;
			needle->suffix = "vp";
			if (!strstr(needle->suffix, type) && strcmp(type,"all")!=0)
				return foundat + size;

			write_to_disk(s, needle, file_size, extractbuf, c_offset + f_offset);
			foundat += file_size;
			return foundat;
		}

		return buf + needle->header_len;

		}
	else
		{
		return buf + needle->header_len;
		}

	return NULL;

}


