#coding=UTF-8



import hashlib



def jinja2_filter_hash(value, hash_type="md5"):
	"""
	Example filter providing custom Jinja2 filter - hash

	Hash type defaults to 'md5' if one is not specified

	:param value: value to be hashed
	:param hash_type: valid hash type
	:return: computed hash as a hexadecimal string
	"""
	hash_func = getattr(hashlib, hash_type, None)

	# 提速版
	computed_hash = hash_func(value.encode("UTF-8")).hexdigest()
	'''
	if hash_func:
		computed_hash = hash_func(value.encode("UTF-8")).hexdigest()
	else:
		raise AttributeError("No hashing function named {hname}".format(hname=hash_type))
	'''
	return computed_hash

