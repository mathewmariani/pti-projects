import { defineCollection, z } from 'astro:content';

const dimensionsSchema = z.object({
	width: z.number(),
	height: z.number(),
	scale: z.number(),
});

const baseSchema = z.object({
	title: z.string(),
	src: z.string(),
	dimensions: dimensionsSchema,
});

export const projects = defineCollection({
	type: 'content',
	schema: baseSchema,
});

export const samples = defineCollection({
	type: 'content',
	schema: baseSchema,
});

export const collections = { projects, samples };