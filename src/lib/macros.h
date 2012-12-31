#ifndef __LIB_MACROS_H_
#define __LIB_MACROS_H_

// Basic comparison and bit-level operators
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define IS_SET(flag, bit) ((flag) & (bit))
#define SET_BIT(val, bit) ((val) |= (bit))
#define REMOVE_BIT(val, bit) ((val) &= ~(bit))

#define CALCTIME(h, m, s) (misc.midnight + h * 3600 + m * 60 + s)

#define INFO(sync, format, args...) log_msg(LOG_INFO, __FUNCTION__, sync, format , ## args);
#define CRIT(sync, format, args...) log_msg(LOG_CRIT, __FUNCTION__, sync, format , ## args);
#define ERROR(sync, format, args...) log_msg(LOG_ERR, __FUNCTION__, sync, format , ## args);
#define DEBUG(sync, format, args...) log_msg(LOG_DEBUG, __FUNCTION__, sync, format , ## args);

// Linked list operations
#define LINKED_LIST_PREALLOC(prealloc_name, data_type, free_list, name) \
	static void prealloc_name(int count) { \
		data_type *list_entry; \
		void *block = NULL; \
		int i; \
		log_msg(LOG_INFO, __FUNCTION__, 0, "Pre-allocating %d %s entries (%d bytes)", count, name, count * sizeof(data_type)); \
		block = malloc(count * sizeof(data_type)); \
		if (!block) { log_msg(LOG_CRIT, __FUNCTION__, 1, "Unable to allocate memory: %s (%d)", strerror(errno), errno); return; } \
		for (i = 0; i < count; i++) { \
			list_entry = (data_type *) (block + sizeof(data_type) * i); \
			list_entry->next = free_list; \
			free_list = list_entry; \
		} \
	}

#define LINKED_LIST_FREE(free_name, data_type, free_list) \
	static inline void free_name(data_type *list_entry) { \
		list_entry->next = free_list; \
		free_list = list_entry; \
	}

#define LINKED_LIST_ALLOCATE(allocate_name, prealloc_name, prealloc_count, data_type, free_list) \
	static inline data_type *allocate_name(void) { \
		data_type *list_entry; \
		if (!free_list) prealloc_name(prealloc_count); \
		list_entry = free_list; \
		free_list = list_entry->next; \
		memset(list_entry, 0, sizeof(data_type)); \
		return list_entry; \
	}

#define LINKED_LIST_KILL(kill_name, data_type, free_name, name) \
	static void kill_name(data_type **list, data_type *e) { \
		data_type *tmp; \
		if (e == *list) { \
			*list = e->next; \
		} else { \
			for (tmp = *list; tmp && tmp->next != e; tmp = tmp->next) \
				; \
			if (tmp) { \
				tmp->next = e->next; \
			} else { \
				log_msg(LOG_ERR, __FUNCTION__, 1, "Serious bug in %s linked list handling.", name); \
			} \
		} \
		free_name(e); \
	}

#define LINKED_LIST_UNLINK(unlink_name, data_type, free_name, name) \
	static void unlink_name(data_type **list, data_type *e) { \
		data_type *tmp; \
		if (e == *list) { \
			*list = e->next; \
		} else { \
			for (tmp = *list; tmp && tmp->next != e; tmp = tmp->next) \
				; \
			if (tmp) { \
				tmp->next = e->next; \
			} else { \
				log_msg(LOG_ERR, __FUNCTION__, 1, "Serious bug in %s linked list handling.", name); \
			} \
		} \
	}

#define LINKED_LIST_INSSORTED(inssorted_name, data_type, compare_entries) \
	static void inssorted_name(data_type **list, data_type *e) { \
		data_type *tmp; \
		if (!*list) { \
			e->next = NULL; \
			*list = e; \
			return; \
		} \
		if (*list && compare_entries(e, *list) <= 0) { \
			e->next = *list; \
			*list = e; \
			return; \
		} \
		for (tmp = *list; tmp && tmp->next; tmp = tmp->next) \
			if (compare_entries(e, tmp->next) <= 0) break; \
		if (!tmp) { \
			ERROR(1, "Fuckup in list handling, could not find element to insert before, prepending"); \
			e->next = *list; \
			*list = e; \
			return; \
		} \
		e->next = tmp->next; \
		tmp->next = e; \
	}

#endif
