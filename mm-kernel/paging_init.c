void __init paging_init(void)
{
	sparse_memory_present_with_active_regions(MAX_NUMNODES);
	sparse_init();

	/*
	 * clear the default setting with node 0
	 * note: don't use nodes_clear here, that is really clearing when
	 *	 numa support is not compiled in, and later node_set_state
	 *	 will not set it back.
	 */
	node_clear_state(0, N_MEMORY);
	if (N_MEMORY != N_NORMAL_MEMORY)
		node_clear_state(0, N_NORMAL_MEMORY);

	zone_sizes_init();
}


void __init sparse_memory_present_with_active_regions(int nid)
{
	unsigned long start_pfn, end_pfn;
	int i, this_nid;

	for_each_mem_pfn_range(i, nid, &start_pfn, &end_pfn, &this_nid)
		memory_present(this_nid, start_pfn, end_pfn);
}

sparse_memory_present_with_active_regions: walks through all memblock.memory blocks and mark them 
memory_present: 
	- walks through pfn, adding PAGES_PER_SECTION
	- get section_nr from pfn
	- gets root from section_nr
	- allocates a mem_section structure and pointing mem_section[root] to it
	- section_to_node_table[section_nr] = nid;
	- set __highest_present_section_nr
	- mark section as SECTION_IS_ONLINE | SECTION_MARKED_PRESENT;

sparse_init:
	- size = sizeof(unsigned long *) * NR_MEM_SECTIONS = 4MB
	- allocate 4MB from memblock and convert it to virt

alloc_usemap_and_memmap:
	- walk through all sections and check wether the section was marked as SECTION_MARKED_PRESENT
	- count number of maps for same nodeid
	- call sparse_early_usemaps_alloc_node
sparse_early_usemaps_alloc_node:
	- get usermap_size
	- allocate memory
	- walk through all sections and if section is present, usemap_map[nr_section] = usemap
	- usemap += size	

 sparse_init_one_section:
	- ms->section_mem_map |= sparse_encode_mem_map(mem_map, pnum) | SECTION_HAS_MEM_MAP;
	- ms->pageblock_flags = pageblock_bitmap; (pageblock_bitmap == usemap_map[section])

for_each_mem_pfn_range(i, nid, &start_pfn, &end_pfn, &this_nid)

for (i = -1, __next_mem_pfn_range(&i, nid, p_start, p_end, p_nid); i >= 0; __next_mem_pfn_range(&i, nid, p_start, p_end, p_nid))



PAGES_PER_SECTION: 32768
NR_MEM_SECTIONS:  524288
