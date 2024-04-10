#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rule {
  char type;     // x, m, a, s
  char operator; // <, >
  int value;
  char *targetWorkflowName;
} rule_t;

typedef struct workflow {
  // store up to 4 rules
  char *name;
  rule_t *rules;
  int rule_count;
  char *final_rule;
  bool is_accepted;
} workflow_t;

typedef struct part {
  int x, m, a, s;
  int part_rating;
} part_t;

int organise_part(part_t *part);
workflow_t *get_workflow(char *name);
bool check_part_rule(int value, rule_t *rule);
void print_workflows();

workflow_t *workflows;
int workflow_count = 0;
part_t *parts;
int part_count = 0;

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];
  bool is_reading_instructions = true;

  while (fgets(line, sizeof(line), input)) {
    if (is_reading_instructions) {
      if (line[0] == '\n') {
        is_reading_instructions = false;
        continue;
      }
      workflow_count++;
    } else {
      part_count++;
    }
  }
  rewind(input);

  workflows = (workflow_t *)malloc(workflow_count * sizeof(workflow_t));
  parts = (part_t *)malloc(part_count * sizeof(part_t));

  is_reading_instructions = true;
  int instruction_index = 0;
  int part_index = 0;
  while (fgets(line, sizeof(line), input)) {
    if (is_reading_instructions) {
      if (line[0] == '\n') {
        is_reading_instructions = false;
        continue;
      }
      // this is an instruction
      char *lineCopy = strdup(line);

      char *workflow_name = strtok(lineCopy, "{");
      printf("Workflow name: %s\n", workflow_name);

      // get the rules
      int rule_count = 0;
      for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ',') {
          rule_count++;
        }
      }
      printf("Rule count: %d\n", rule_count);
      printf("Name: %s\n", workflow_name);

      // this works up to here
      // get each rule
      workflow_t workflow;
      workflow.name = (char *)malloc(strlen(workflow_name) * sizeof(char));
      workflow.rules = (rule_t *)malloc(rule_count * sizeof(rule_t));
      rule_t rules[rule_count];
      for (int i = 0; i < rule_count; i++) {
        char *rule = strtok(NULL, ":");
        printf("Rule: %s\n", rule);
        workflow.rules[i].type = rule[0];
        printf("Instruction type: %c\n", workflow.rules[i].type);
        workflow.rules[i].operator= rule[1];
        printf("Operator: %c\n", workflow.rules[i].operator);
        workflow.rules[i].value = atoi(&rule[2]);
        printf("Value: %d\n", workflow.rules[i].value);
        printf("Rule: %c %c %d\n", workflow.rules[i].type,
               workflow.rules[i].operator, workflow.rules[i].value);

        // get the target workflow
        char *str = strtok(NULL, ",");
        printf("Str: %s\n", str);
        workflow.rules[i].targetWorkflowName = strdup(str);
      }
      printf("Rule count: %d\n", rule_count);
      workflow.rule_count = rule_count;
      workflow.final_rule = strdup(strtok(NULL, "}"));
      printf("Final rule: %s\n", workflow.final_rule);
      printf("%s\n", workflow_name);
      strcpy(workflow.name, workflow_name);
      // workflow.rule_count = 0;
      printf("Instruction count: %d\n", instruction_index);
      printf("Instruction name: %s\n", workflows[instruction_index].name);
      workflows[instruction_index++] = workflow;
    } else {
      char *name = strtok(line, "{");
      printf("Name: %s\n", name);
      char *x = strtok(name, ",");
      printf("X: %s\n", x);
      char *m = strtok(NULL, ",");
      char *a = strtok(NULL, ",");
      char *s = strtok(NULL, ",");
      int intx = 0, intm = 0, inta = 0, ints = 0;
      intx = atoi(&x[2]);
      intm = atoi(&m[2]);
      inta = atoi(&a[2]);
      ints = atoi(&s[2]);
      int part_rating = intx + intm + inta + ints;
      parts[part_index++] = (part_t){intx, intm, inta, ints, part_rating};
      printf("X: %d, M: %d, A: %d, S: %d | T: %d\n", intx, intm, inta, ints,
             part_rating);
    }
  }
  print_workflows();
  printf("Instruction count: %d\n", workflow_count);
  for (int i = 0; i < instruction_index; i++) {
    printf("Instruction %d: %s\n", i, workflows[i].name);
  }
  printf("Part count: %d\n", part_index);
  for (int i = 0; i < part_count; i++) {
    printf("Part %d: X: %d, M: %d, A: %d, S: %d, total: %d\n", i, parts[i].x,
           parts[i].m, parts[i].a, parts[i].s, parts[i].part_rating);
  }

  // to compute the number we need to iterate through the parts
  workflow_t *first_workflow = get_workflow("in");
  int total = 0;
  for (int i = 0; i < part_count; i++) {
    printf("Part %d\n===============\n", i + 1);
    printf("X: %d, M: %d, A: %d, S: %d\n", parts[i].x, parts[i].m, parts[i].a,
           parts[i].s);
    total += organise_part(&parts[i]);
    printf("Total: %d\n", total);
  }
  printf("FINAL total: %d\n", total);
}

int organise_part(part_t *part) {
  // get the first workflow
  workflow_t *current_workflow = get_workflow("in");

  int total = 0;
  bool part_completed = false;
  while (!part_completed) {
    bool rule_met = false;
    printf("Rule count: %d\n", current_workflow->rule_count);
    // printf("Current workflow: %s\n", current_workflow->name);
    for (int i = 0; i < current_workflow->rule_count; i++) {
      if (rule_met) {
        i = 0;
        rule_met = false;
      }
      printf("Workflow: %s\nRule: %d\n", current_workflow->name, i);
      if (strcmp(current_workflow->name, "qqz") == 0) {
        for (int j = 0; j < current_workflow->rule_count; j++) {
          printf("Rule %d: %c %c %d\n", j, current_workflow->rules[j].type,
                 current_workflow->rules[j].operator,
                 current_workflow->rules[j].value);
        }
      }
      rule_t rule = current_workflow->rules[i];
      printf("rule -> %c %c %d\n", rule.type, rule.operator, rule.value);
      if (rule.type == 'x') {
        if (check_part_rule(part->x, &rule)) {
          printf("X\n");
          printf("Target workflow: %s\n", rule.targetWorkflowName);
          rule_met = true;
        }
      } else if (rule.type == 'm') {
        if (check_part_rule(part->m, &rule)) {
          printf("M\n");
          printf("Target workflow: %s\n", rule.targetWorkflowName);
          rule_met = true;
        }
      } else if (rule.type == 'a') {
        if (check_part_rule(part->a, &rule)) {
          printf("A\n");
          printf("Target workflow: %s\n", rule.targetWorkflowName);
          rule_met = true;
        }
      } else if (rule.type == 's') {
        if (check_part_rule(part->s, &rule)) {
          printf("S\n");
          printf("Target workflow: %s\n", rule.targetWorkflowName);
          rule_met = true;
        }
      }
      printf("Rule type: %c\n", rule.type);
      if (rule_met) {
        if (strcmp(rule.targetWorkflowName, "A") == 0) {
          printf("Returning part rating: %d\n", part->part_rating);
          return part->part_rating;
        } else if (strcmp(rule.targetWorkflowName, "R") == 0) {
          printf("Workflow %s rejected\n", current_workflow->name);
          return 0;
        } else {
          current_workflow = get_workflow(rule.targetWorkflowName);
        }
      }
      // if we reach the end of the loop, we have to check the final rule
      if (strcmp(current_workflow->final_rule, "A") == 0) {
        return part->part_rating;
      } else if (strcmp(current_workflow->final_rule, "R") == 0) {
        printf("Workflow %s rejected\n", current_workflow->name);
        return 0;
      } else {
        current_workflow = get_workflow(current_workflow->final_rule);
      }
    }
  }
  return 0;
}

bool check_part_rule(int value, rule_t *rule) {
  printf("Checking rule: %c %c %d\n", rule->type, rule->operator, rule->value);
  printf("Value: %d\n", value);
  if (rule->operator== '<') {
    return value < rule->value;
  } else {
    return value > rule->value;
  }
}

workflow_t *get_workflow(char *name) {
  for (int i = 0; i < workflow_count; i++) {
    if (strcmp(workflows[i].name, name) == 0) {
      return &workflows[i];
    }
  }
  return NULL;
}

void print_workflows() {
  printf("PRINTING\n-----------------\n");
  printf("Workflow count: %d\n", workflow_count);
  for (int i = 0; i < workflow_count; i++) {
    for (int j = 0; j < workflows[i].rule_count; j++) {
      printf("Rule %d: %c %c %d\n", j, workflows[i].rules[j].type,
             workflows[i].rules[j].operator, workflows[i].rules[j].value);
      printf("Target workflow: %s\n", workflows[i].rules[j].targetWorkflowName);
    }
    printf("Final rule: %s\n", workflows[i].final_rule);
  }
}
