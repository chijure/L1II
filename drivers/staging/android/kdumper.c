/*
 * kdumper.c - logger messages redirection to kmesg
 *
 *  Copyright (C) Intel 2012
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

//#define pr_fmt(fmt) "ADRD: " fmt

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include "logger.h"

MODULE_DESCRIPTION("Android Log kernel dumper");
MODULE_AUTHOR("Robert Chiras");
MODULE_LICENSE("GPL");

#define MAX_BUF_SZ (LOGGER_ENTRY_MAX_PAYLOAD + 1)
#define LOG_PRINT_LVL 5  /* WARN,ERR,FATAL */
static DEFINE_SPINLOCK(write_lock);

static const char *const logs_to_dump[] = {
	"main",
	"radio",
	"system",
};


static int logger_mask;
int android_logger_mask(void)
{
	return logger_mask;	
}
EXPORT_SYMBOL(android_logger_mask);

static int atoi(const char *name)
{
	int val = 0;
	for (;; name++) {
		switch (*name) {
		case '0' ... '9':
			val = 10*val+(*name-'0');
			break;
		default:
			return val;
		}
	}
}

static int __init lge_set_android_logger_mask(char *mask)
{
	logger_mask = atoi(mask);
	printk(" Andorid logger mask %d\n", logger_mask);
	return 1;
}
__setup("lge.loggermask=",lge_set_android_logger_mask);

static void print_android_log(const char *log_name,
			      const char *buf, size_t count, bool last_dump)
{

	static char temp_buf[MAX_BUF_SZ] __aligned(4);
	static char *last_pos;
	char *tag;
	int i;
	bool do_write = false;
	unsigned char log_print_pri = 0;

	/* Skip the "log_" from log_name */
	log_name += 4;

	for (i = 0; i < ARRAY_SIZE(logs_to_dump); i++) {
		if (!strncmp(log_name, logs_to_dump[i], strlen(logs_to_dump[i]))) {
			if( (1 << i) & logger_mask ){
				do_write = true;
				break;
			}
		}
	}

	if (!do_write)
		return;

	spin_lock(&write_lock);

	if (!last_dump) {
		/* This is the first chunk, initialize buffer and cursor */
		if (last_pos == NULL) {
			last_pos = temp_buf;
			memset(temp_buf, 0, MAX_BUF_SZ);
		}
		/* Check for buffer overflow */
		count = min_t(size_t, count, (temp_buf + MAX_BUF_SZ) - last_pos);
		if (count > 0) {
			memcpy(last_pos, buf, count);
			last_pos += count;
			spin_unlock(&write_lock);
			return;
		}
	}

	if (last_pos == NULL) {
		/* No previous chunks, initialize buffer and cursor */
		memset(temp_buf, 0, MAX_BUF_SZ);
		memcpy(temp_buf, buf, count);
	}

	/*
	 * format: <priority:1><tag:N>\0<message:N>\0
	 *
	 * tag str
	 *   starts at buf+1
	 * msg
	 *   starts at buf+1+len(tag)+1
	 */
	log_print_pri = (unsigned char)temp_buf[0];
	tag = temp_buf + 1;
	if( LOG_PRINT_LVL <= log_print_pri )
		pr_info("%s : [%s]: %s\n",log_name,tag,tag + strlen(tag) + 1);

	last_pos = NULL;

	spin_unlock(&write_lock);
}

static struct aux_logger_device kdumper = {
	.write_log_entry = print_android_log
};

static int kdumper_init(void)
{
	return register_aux_logger(&kdumper);
}

static void kdumper_exit(void)
{
	unregister_aux_logger(&kdumper);
}

module_init(kdumper_init);
module_exit(kdumper_exit);
