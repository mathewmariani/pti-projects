import { defineCollection, z } from 'astro:content';

const projects = defineCollection({
	type: 'content',
	schema: z.object({
		title: z.string(),
		src: z.string(),
	}),
});

const samples = defineCollection({
	type: 'content',
	schema: z.object({
		title: z.string(),
		src: z.string(),
	}),
});

export const collections = { projects, samples };