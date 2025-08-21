from datasets import Dataset
from transformers import (
    AutoTokenizer,
    AutoModelForCausalLM,
    TrainingArguments,
    Trainer,
    DataCollatorForLanguageModeling,
)
import json


with open("pills.json", "r") as f:
    data = json.load(f)


dataset = Dataset.from_list(data)


model_name = "gpt2"  
tokenizer = AutoTokenizer.from_pretrained(model_name)


tokenizer.pad_token = tokenizer.eos_token


def tokenize(batch):
    instructions = batch.get("instruction", [])
    inputs = batch.get("input", [""] * len(instructions))
    outputs = batch.get("output", [""] * len(instructions))

    prompts = []
    for instr, inp, out in zip(instructions, inputs, outputs):
        merged_instruction = instr if not inp else f"{instr}\n{inp}"
        text = f"### Instruction:\n{merged_instruction}\n\n### Response:\n{out}"
        prompts.append(text)

    return tokenizer(
        prompts,
        truncation=True,
        padding="max_length",
        max_length=512,
    )


tokenized_dataset = dataset.map(tokenize, batched=True)
 

args = TrainingArguments(
    output_dir="results",
    per_device_train_batch_size=1,
    num_train_epochs=3,
    logging_steps=10,
    save_steps=50,
    save_total_limit=2
)


model = AutoModelForCausalLM.from_pretrained(model_name)

# Ensure labels are created for causal LM training
data_collator = DataCollatorForLanguageModeling(tokenizer=tokenizer, mlm=False)

trainer = Trainer(
    model=model,
    args=args,
    train_dataset=tokenized_dataset,
    data_collator=data_collator,
)

trainer.train()
