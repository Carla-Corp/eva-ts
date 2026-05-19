export const errors: Record<number, any> = {
    1: function(self: any): string {
        return `Can\'t open file: ${self.filename}`
    },
}
